#pragma once
#include <cmath>
#include <cfloat>

typedef float float_t;

struct vec_t
{
	float_t x, y;
	const vec_t operator+(const vec_t v) const
	{
		return{ x + v.x, y + v.y };
	}
	const vec_t operator+=(const vec_t v)
	{
		x += v.x; y += v.y;
		return *this;
	}
	const vec_t operator-(const vec_t v) const
	{
		return{ x - v.x, y - v.y };
	}
	const vec_t operator-=(const vec_t v)
	{
		x -= v.x; y -= v.y;
		return *this;
	}
	const vec_t operator*(const float_t s) const
	{
		return{ x*s, y*s };
	}
	const vec_t operator*=(const float_t s)
	{
		x *= s; y *= s;
		return *this;
	}
	friend vec_t operator*(const float_t s, vec_t v)
	{
		return{ v.x*s, v.y*s };
	}
};


