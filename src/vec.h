#pragma once
#include <boost/geometry.hpp>
#include <boost/geometry/geometries/register/point.hpp>

#define _USE_MATH_DEFINES
#include <math.h>
#include <cfloat>


template<typename float_t>
struct _vec_t
{
	float_t x, y;
	const _vec_t operator+(const _vec_t v) const
	{
		return{ x + v.x, y + v.y };
	}
	const _vec_t operator+=(const _vec_t v)
	{
		x += v.x; y += v.y;
		return *this;
	}
	const _vec_t operator-(const _vec_t v) const
	{
		return{ x - v.x, y - v.y };
	}
	const _vec_t operator-=(const _vec_t v)
	{
		x -= v.x; y -= v.y;
		return *this;
	}
	const _vec_t operator*(const float_t s) const
	{
		return{ x*s, y*s };
	}
	const _vec_t operator*=(const float_t s)
	{
		x *= s; y *= s;
		return *this;
	}
	const float_t dist(const _vec_t v)
	{
		return sqrt((x-v.x)*(x-v.x)+(y-v.y)*(y-v.y));
	}
	const float_t length()
	{
		return sqrt(x*x + y*y);
	}
	_vec_t normalize()
	{
		float_t l = length();
		if (fabs(l)!=0)
		{
			x /= l; y /= l;
		}
		return {x, y};
	}
	_vec_t saturate(float bound)
	{
		float_t l = length();
		if (fabs(l) != 0)
		{
			x /= l, y /= l;
			*this *= atan(l) / M_PI_2 * bound;
		}
		return{ x, y };
	}
	friend _vec_t operator*(const float_t s, _vec_t v)
	{
		return{ v.x*s, v.y*s };
	}
	float_t cross(_vec_t v)
	{
		return x*v.y - y*v.x;
	}
	float_t dot(_vec_t v)
	{
		return x*v.x + y*v.y;
	}
	_vec_t perp()
	{
		return{ -y, x };
	}
};

typedef _vec_t<float>  vec_t;
typedef _vec_t<double>  vecd_t;

BOOST_GEOMETRY_REGISTER_POINT_2D(vec_t, float, boost::geometry::cs::cartesian, x, y);
BOOST_GEOMETRY_REGISTER_POINT_2D(vecd_t, double, boost::geometry::cs::cartesian, x, y);
