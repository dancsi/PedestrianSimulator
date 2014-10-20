#pragma once

#include "field.h"
#include "geometry.h"
#include "objective.h"

#include <vector>

namespace world
{
	extern int width, height, people_left;
	extern float timestep;
	extern scalar_field_t dist_field[2];
	extern vec_field_t dist_field_grad[2];
	extern std::vector<objective_t> objectives;
	extern std::vector<line_t> walls;
	extern int step_counter, allowed_steps;

	void init();
	void read_walls(std::string fname);
	void setup_neighbours(scalar_field_t& dist_field, vec_field_t& dist_field_grad, matrix_t<vec_t>& prev_pos);
	void draw();
	void add_objective(objective_t obj);
	void place_people_randomly(size_t n_people, box_t& where, short objective_color);
	void step(float dt);
	void destroy();
	bool visible(vec_t a, vec_t b);
}