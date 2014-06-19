#pragma once
#include <cmath>
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
	friend _vec_t operator*(const float_t s, _vec_t v)
	{
		return{ v.x*s, v.y*s };
	}
};

typedef _vec_t<float>  vec_t;
typedef _vec_t<double>  vecd_t;

