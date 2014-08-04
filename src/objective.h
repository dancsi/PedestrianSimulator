#pragma once

#include "vec.h"

struct objective_t :vec_t
{
	short color;
	objective_t() = default;
	objective_t(vec_t pos, short color) : vec_t(pos), color(color) {}
};