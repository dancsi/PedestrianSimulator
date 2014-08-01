#include "world.h"
#include "config.h"
#include "util.h"
#include "vec.h"
#include "graphics.h"
#include "pedestrian.h"

#include <limits>
#include <set>
#include <algorithm>

using namespace std;
using namespace util;

namespace world
{
	int width, height, n, m;
	scalar_field_t dist_field;
	vec_field_t dist_field_grad;
	vector<vec_t> objectives;
	matrix_t<bool> visited;
	matrix_t<vec_t> prev_pos;
	matrix_t<vector<pair_t>> neighbours;
	vector<ped_t> people, inactive_people;
	vector<line_t> walls;

	void init()
	{
		width = config::get<int>("world", "width");
		height = config::get<int>("world", "height");
		float spacing = config::get<double>("world", "spacing");
		dist_field = scalar_field_t({ width, height }, spacing); dist_field.set(std::numeric_limits<float>::infinity());
		dist_field_grad = vec_field_t({ width, height }, spacing); dist_field_grad.clear();
		n = dist_field.n, m = dist_field.m;
		visited = util::matrix_t<bool>(n, m);
		std::string walls_file = config::get<std::string>("data", "walls");
		read_walls(walls_file);
		setup_neighbours();
		if (config::get<int>("people", "random_placement"))
		{
			place_people_randomly(config::get<int>("people", "count"));
		}
		else
		{
			people.push_back(ped_t{70, 35, 0});
		}
	}

	void read_walls(std::string fname)
	{
		FILE* fwalls = fopen(fname.c_str(), "r");
		line_t wall;
		while (fscanf(fwalls, "%f%f%f%f", &wall.p.x, &wall.p.y, &wall.q.x, &wall.q.y) == 4)
		{
			walls.push_back(wall);
		}
		fclose(fwalls);
	}

	void setup_neighbours()
	{
		prev_pos = matrix_t<vec_t>(n, m);
		prev_pos.foreach_element([&](size_t i, size_t j, vec_t& v) {v = dist_field.get_coordinates(i, j); });
		neighbours = matrix_t<vector<pair_t>>(n, m);
		auto point_on_wall = matrix_t<bool>(n, m);
		for (int i = 0; i < n; i++)
		{
			for (int j = 0; j < m; j++)
			{
				vec_t point_pos = dist_field_grad.get_coordinates(i, j);
				point_on_wall.f[i][j] = !all_of(walls.begin(), walls.end(), [&point_pos](line_t wall) {return !covered_by(point_pos, wall); });
			}
		}

		for (int i = 0; i < n; i++)
		{
			//pair_t lpoint{ i, 0 }, rpoint{i, m-1};
			neighbours.f[i][0].push_back({ i, 1 });
			neighbours.f[i][m - 1].push_back({ i, m - 2 });
		}
		for (int j = 0; j < m; j++)
		{
			neighbours.f[0][j].push_back({ 1, j });
			neighbours.f[n - 1][j].push_back({ n - 2, j });
		}
		for (int i = 1; i < n - 1; i++)
		{
			for (int j = 1; j < m - 1; j++)
			{
				if (point_on_wall.f[i][j]) continue;
				if (!point_on_wall.f[i - 1][j]) neighbours.f[i][j].push_back({ i - 1, j });
				if (!point_on_wall.f[i][j + 1]) neighbours.f[i][j].push_back({ i, j + 1 });
				if (!point_on_wall.f[i + 1][j]) neighbours.f[i][j].push_back({ i + 1, j });
				if (!point_on_wall.f[i][j - 1]) neighbours.f[i][j].push_back({ i, j - 1 });
			}
		}
	}

	void place_people_randomly(size_t n_people)
	{
		for (int i = 0; i < n_people; i++)
		{
			ped_t p = { util::rand_range(width * 0.1, width*.9), util::rand_range(world::height * 0.1, world::height*.9), util::rand_range(0, 2 * M_PI) };
			people.push_back(p);
		}
	}

	void draw()
	{
		graphics::draw(dist_field_grad);
		for (vec_t obj : objectives)
		{
			nvgBeginPath(graphics::vg);
			nvgCircle(graphics::vg, obj.x, obj.y, 0.5);
			nvgFillColor(graphics::vg, nvgRGBAf(1, 0, 0, .5));
			nvgFill(graphics::vg);
		}
		for (ped_t ped : world::people)
		{
			nvgBeginPath(graphics::vg);
			nvgCircle(graphics::vg, ped.x, ped.y, 0.5);
			nvgFillColor(graphics::vg, nvgRGBAf(0, 1, 0, .5));
			nvgFill(graphics::vg);
		}
		for (line_t& wall : walls)
		{
			nvgBeginPath(graphics::vg);
			nvgMoveTo(graphics::vg, wall.p.x, wall.p.y);
			nvgLineTo(graphics::vg, wall.q.x, wall.q.y);
			nvgStrokeColor(graphics::vg, nvgRGBAf(0, 0, 1, .5));
			nvgStrokeWidth(graphics::vg, 3 * graphics::one_pixel);
			nvgStroke(graphics::vg);
		}
	}

	struct astar_cmp
	{
		static vec_t start;
		//astar_cmp(vec_t start) :start(start) {}
		float_t euclidean_dist(pair_t x)
		{
			return start.dist(dist_field.get_coordinates(x));
		}
		const bool operator()(pair_t a, pair_t b)
		{
			return dist_field[a] + euclidean_dist(a) < dist_field[b] + euclidean_dist(b);
		}
	};
	vec_t astar_cmp::start;
	std::multiset < pair_t, astar_cmp > q;

	bool visible(vec_t a, vec_t b)
	{
		line_t sight_ray{ a, b };
		return all_of(walls.begin(), walls.end(), [&sight_ray](line_t& wall) {return !intersects(sight_ray, wall); });
	}

	void point_grad_vec_to_prev_pos(pair_t p) //usmerava
	{
		vec_t pos = dist_field.get_coordinates(p);
		vec_t dir = (prev_pos[p] - pos);
		dist_field_grad[p] = dir.normalize();
	}

	void relax(pair_t to, pair_t via)
	{
		bool b = false;
		vec_t to_pos = dist_field.get_coordinates(to), via_pos = dist_field.get_coordinates(via);
		float_t newdist;
		vec_t possible_prev_pos;
		if (visible(prev_pos[via], to_pos))
		{
			newdist = to_pos.dist(prev_pos[via]);
			if (visible(prev_pos[via], astar_cmp::start))
			{
				 newdist+= prev_pos[via].dist(astar_cmp::start); //ALI STA AKO SE NE VIDI prev_pos[via] iz start-a ?????  Trebalo naci cvor koji je najblizi prev_pos[via]
				 possible_prev_pos = prev_pos[via];
			}
			else
			{
				auto closest_points = dist_field.get_ijv(prev_pos[via]);
				float min_dist = numeric_limits<float>::infinity();
				for (pair_t p : closest_points)
				{
					if (dist_field[p] < min_dist)
					{
						min_dist = dist_field[p];
						possible_prev_pos = dist_field.get_coordinates(p);
					}
				}
				newdist += min_dist;
			}

			
		}
		else
		{
			newdist = dist_field[via] + to_pos.dist(via_pos);
			possible_prev_pos = via_pos;
		}
		if (newdist < dist_field[to])
		{
			q.erase(to);
			dist_field[to] = newdist;
			q.insert(to);
			prev_pos[to] = possible_prev_pos;
			point_grad_vec_to_prev_pos(to);
		}
	}

	void astar(vec_t start)
	{
		visited.clear();
		auto startv = dist_field.get_ijv(start); astar_cmp::start = start;
		for (pair_t p : startv)
		{
			float_t newdist = start.dist(dist_field.get_coordinates(p));
			if (newdist < dist_field[p])
			{
				dist_field[p] = newdist;
				prev_pos[p] = start; point_grad_vec_to_prev_pos(p);
				q.insert(p);
			}
		}
		while (!q.empty())
		{
			pair_t t = *q.begin();
			q.erase(q.begin()); visited[t] = true;
			for (pair_t p : neighbours[t])
			{
				if (visited[p]) continue;
				relax(p, t);
			}
		}
	}

	void add_objective(vec_t obj)
	{
		astar(obj);
		objectives.push_back(obj);
	}

	void enforce_boundaries()
	{
		for (ped_t& ped : people)
		{
			if (ped.x < 0)
			{
				ped.x = 0.1;
				ped.v.x = 0;
			}
			if (ped.x > world::width)
			{
				ped.x = world::width - 0.1;
				ped.v.x = 0;
			}
			if (ped.y < 0)
			{
				ped.y = 0.1;
				ped.v.y = 0;
			}
			if (ped.y > world::height)
			{
				ped.y = world::height - 0.1;
				ped.v.y = 0;
			}
		}
	}

	void step(float dt)
	{
		for (ped_t& ped : people)
		{
			ped.acc = dist_field_grad.interpolate(ped);
			for (vec_t obj : objectives)
			{
				vec_t r = obj - ped;
				float r_length = r.length();
				if (r_length < dist_field_grad.spacing / 2)
				{
					ped.arrived_at_destination = true;
				}
				ped.acc += (10. / r_length)*exp(-sqr(r_length - 1.0*dist_field_grad.spacing))*r;
			}
		}


		for (ped_t& ped : people)
		{
			ped.v.saturate(3);
			vec_t newpos = ped + ped.v*dt + 0.5*ped.acc*dt*dt;
			//ped += ped.v*dt + 0.5*ped.acc*dt*dt;
			for (line_t wall : walls)
			{
				vector<vec_t> collision_points; line_t movement_line{ ped, newpos };
				intersection(wall, movement_line, collision_points);
				if (!collision_points.empty())
				{
					vec_t norm = wall.q - wall.p; norm = norm.perp(); norm.normalize();
					if (norm.dot(ped.v) < 0)
					{
						vec_t v_proj = norm.dot(ped.v)*norm;
						ped.v -= v_proj; ped.v += 0.5*norm;
					}
					//LOG("v: (%.2f, %.2f), a: (%.2f, %.2f), line: (%.2f, %.2f) -> (%.2f, %.2f)", ped.v.x, ped.v.y, ped.acc.x, ped.acc.y, ped.x, ped.y, newpos.x, newpos.y);
					if (norm.dot(ped.acc) < 0)
					{
						vec_t acc_proj = norm * norm.dot(ped.acc);
						ped.acc -= acc_proj;
					}

					if (!covered_by((vec_t)ped, wall))
						newpos = collision_points[0];
				}
			}
			ped = newpos;
			ped.v += ped.acc*dt;
		}
		enforce_boundaries();
	}

}