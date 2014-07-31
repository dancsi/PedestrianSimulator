#pragma once

#include "vec.h"

#include <boost/geometry/geometries/segment.hpp> 
#include <boost/geometry/geometries/register/segment.hpp>
#include <boost/geometry/algorithms/intersection.hpp>
#include <boost/geometry/algorithms/covered_by.hpp>

struct line_t
{
	vec_t p, q;
};

BOOST_GEOMETRY_REGISTER_SEGMENT(line_t, vec_t, p, q);

using boost::geometry::intersects;
//typedef boost::geometry::model::segment<vec_t> line_t;