#include "world.h"
#include "config.h"
#include "util.h"
#include "vec.h"
#include "graphics.h"

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

	void init()
	{
		width = config::get<int>("world", "width");
		height = config::get<int>("world", "height");
		float spacing = config::get<double>("world", "spacing");
		dist_field = scalar_field_t({ width, height }, spacing); dist_field.set(std::numeric_limits<float>::infinity());
		dist_field_grad = vec_field_t({ width, height }, spacing); dist_field_grad.clear();
		n = dist_field.n, m = dist_field.m;
		visited = util::matrix_t<bool>(n, m);
		setup_neighbours();
	}

	void setup_neighbours()
	{
		prev_pos = matrix_t<vec_t>(n, m);
		prev_pos.foreach_element([&](size_t i, size_t j, vec_t& v) {v = dist_field.get_coordinates(i, j); });
		neighbours = matrix_t<vector<pair_t>>(n, m);
		for (int i = 0; i < n; i++)
		{
			//pair_t lpoint{ i, 0 }, rpoint{i, m-1};
			neighbours.f[i][0].push_back({i, 1});
			neighbours.f[i][m-1].push_back({ i, m-2 });
		}
		for (int j = 0; j < m; j++)
		{
			neighbours.f[0][j].push_back({1, j});
			neighbours.f[n-1][j].push_back({ n-2, j });
		}
		for (int i = 1; i < n - 1; i++)
		{
			for (int j = 1; j < m - 1; j++)
			{
				neighbours.f[i][j].push_back({ i - 1, j });
				neighbours.f[i][j].push_back({ i, j + 1 });
				neighbours.f[i][j].push_back({ i + 1, j });
				neighbours.f[i][j].push_back({ i, j - 1 });
			}
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
	std::set < pair_t, astar_cmp > q;

	bool visible(vec_t a, vec_t b)
	{
		return true;
	}

	void point_grad_vec_to_prev_pos(pair_t p)
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
			newdist = to_pos.dist(prev_pos[via]) + prev_pos[via].dist(astar_cmp::start); //ALI STA AKO SE NE VIDI prev_pos[via] iz start-a ?????  Trebalo naci cvor koji je najblizi prev_pos[via]
			possible_prev_pos = prev_pos[via];
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
			pair_t t = *q.begin(); q.erase(q.begin()); visited[t] = true;
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

}