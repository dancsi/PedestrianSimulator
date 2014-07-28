#pragma once

#include "vec.h"

#include <cfloat>
#include <initializer_list>

struct line_t
{
	const vec_t p, q;
	vec_t a, v;

	line_t(vec_t p, vec_t  q) :p(p), q(q) 
	{
		a = p;
		v = q - p;
	}
	line_t(std::initializer_list<vec_t> l) :line_t(*l.begin(), *(l.begin()+1)) {}

	short classify_point(vec_t x)
	{
		float f = (x - p).cross(q-p);
		if (abs(f) == 0)
		{
			float T = (x - p).x / v.x;
			return T >= 0 && T <= 1;
		}
		if (f < 0) return -1;
		return +1;
	}
	bool intersects(line_t l)
	{
		short pa = classify_point(l.p), pb = classify_point(l.q);
		if (pa*pb > 0) return 0;

	}
};