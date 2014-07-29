#pragma once

#include "field.h"
#include "geometry.h"

#include <vector>

namespace world
{
	extern int width, height;
	extern scalar_field_t dist_field;
	extern vec_field_t dist_field_grad;
	extern std::vector<vec_t> objectives;

	void init();
	void setup_neighbours();
	void draw();
	void add_objective(vec_t obj);
}