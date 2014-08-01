#pragma once

#include "vec.h"

#include <boost/geometry.hpp>
#include <boost/geometry/geometries/segment.hpp> 
#include <boost/geometry/geometries/register/segment.hpp>
#include <boost/geometry/algorithms/intersection.hpp>
#include <boost/geometry/algorithms/covered_by.hpp>
#include <boost/geometry/algorithms/within.hpp>

struct line_t
{
	vec_t p, q;
};

BOOST_GEOMETRY_REGISTER_SEGMENT(line_t, vec_t, p, q);

struct circle_t :vec_t
{
	float r;
	circle_t(std::initializer_list<float> l) : vec_t({ *(l.begin()), *(l.begin() + 1) }), r(*(l.begin() + 2)) {}
};

using boost::geometry::intersects;
using boost::geometry::intersection;
using boost::geometry::intersects;
using boost::geometry::covered_by;
using boost::geometry::within;
//typedef boost::geometry::model::segment<vec_t> line_t;