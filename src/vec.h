#pragma once
#include <cmath>
#include <cfloat>

struct vec_t
{
	typedef float num;
	num x, y;
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
	const vec_t operator*(const num s) const
	{
		return{ x*s, y*s };
	}
	const vec_t operator*=(const num s)
	{
		x *= s; y *= s;
		return *this;
	}
	friend vec_t operator*(const num s, vec_t v)
	{
		return{ v.x*s, v.y*s };
	}
};


