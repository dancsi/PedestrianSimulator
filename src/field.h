#pragma once

#include "vec.h"
#include "util.h"


#include <functional>
#include <random>
#define _USE_MATH_DEFINES
#include <math.h>
#include <vector>

using namespace util;

template <typename T>
struct field_t : matrix_t<T>
{
	vec_t size;
	float spacing;
	field_t(vec_t size, float spacing) : size(size), spacing(spacing), matrix_t(size.y / spacing + 1, size.x / spacing + 1)
	{	}

	vec_t get_coordinates(size_t i, size_t j)
	{
		return spacing*vec_t{ j, i };
	}

	vec_t get_coordinates(pair_t ij)
	{
		return spacing*vec_t{ ij.j, ij.i };
	}

	pair_t get_ij(vec_t v)
	{
		return{ size_t(v.y / spacing), size_t(v.x / spacing) };
		
	}
	std::vector<pair_t> get_ijv(vec_t v)
	{
		pair_t ij = get_ij(v);
		size_t i = ij.i, j = ij.j;
		return{ { i, j }, { i + 1, j }, { i, j + 1 }, {i+1, j+1} };
	}

	field_t()  {}

	void operator*=(float_t s)
	{
		for (size_t i = 0; i < n; i++)
		{
			for (size_t j = 0; j < m; j++)
			{
				f[i][j] *= s;
			}
		}
	}

	void operator+=(T x)
	{
		for (size_t i = 0; i < n; i++)
		{
			for (size_t j = 0; j < m; j++)
			{
				f[i][j] += x;
			}
		}
	}
	float W(float dist)
	{
		return exp(-dist*dist);
	}
	T interpolate(vec_t pos, std::function<bool(vec_t, vec_t)> can_include_point)
	{
		T res = {0, 0};
		auto&& cell_corners = get_ijv(pos);
		for (pair_t& ij : cell_corners)
		{
			if (can_include_point(get_coordinates(ij), pos))
			{
				vec_t r = (pos - get_coordinates(ij));
				res += W(r.length())*f[ij.i][ij.j];
			}	
		}
		return res.normalize();
	}
};
 
//template<typename T>
struct vec_field_t : field_t<vec_t>
{
	vec_field_t(vec_t size, float spacing) :field_t(size, spacing)
	{
		std::mt19937 eng(42);
		auto rnd = bind(std::normal_distribution<float>(0, 0.3), eng);

		for (size_t i = 0; i < n; i++)
		{
			for (size_t j = 0; j < m; j++)
			{
				float angle = M_PI_4+rnd();
				f[i][j] = { cos(angle), sin(angle) };
			}
		}
	}
	vec_field_t(){}
};

struct scalar_field_t : field_t < float_t >
{
	scalar_field_t(vec_t size, float spacing) :field_t(size, spacing) {	}
	scalar_field_t() {}
};

//using vec_field_t = field_t < vec_t > ;