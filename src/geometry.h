#pragma once

#include "vec.h"

#include <boost/geometry/geometries/segment.hpp> 
#include <boost/geometry/geometries/register/segment.hpp>
#include <boost/geometry/algorithms/intersection.hpp>

struct line_t
{
	const vec_t p, q;
	vec_t a, v;

	line_t(vec_t p, vec_t  q) :p(p), q(q)
	{
		a = p;
		v = q - p;
	}
	line_t(std::initializer_list<vec_t> l) :line_t(*l.begin(), *(l.begin() + 1)) {}
};

BOOST_GEOMETRY_REGISTER_SEGMENT(line_t, vec_t, p, q);

using boost::geometry::intersects;
//typedef boost::geometry::model::segment<vec_t> line_t;