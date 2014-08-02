#pragma once

#include "field.h"
#include "geometry.h"

#include <vector>

namespace world
{
	extern int width, height;
	extern float timestep;
	extern scalar_field_t dist_field;
	extern vec_field_t dist_field_grad;
	extern std::vector<vec_t> objectives;
	extern std::vector<line_t> walls;

	void init();
	void read_walls(std::string fname);
	void setup_neighbours(); 
	void draw();
	void add_objective(vec_t obj);
	void place_people_randomly(size_t n_people, box_t& where);
	void step(float dt);
	void destroy();
	bool visible(vec_t a, vec_t b);
}