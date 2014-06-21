#pragma once

#include "vec.h"
#include "util.h"

#include <functional>
#include <random>

template <typename T>
struct field_t
{
	size_t n, m;
	vec_t size;
	float spacing;
	T **f;
	field_t(vec_t size, float spacing) : size(size), spacing(spacing)
	{
		n = size.y / spacing;
		m = size.x / spacing;
		T* tmp = new T[n*m];
		f = new T*[n];
		for (int i = 0; i < n; i++) f[i] = tmp + i*m;
	}

	vec_t get_coordinates(size_t i, size_t j)
	{
		return spacing*vec_t{ j, i };
	}

	void foreach_row(std::function<void(size_t i, T*& row)> fun)
	{
		for (size_t i = 0; i < n; i++) fun(i, f[i]);
	}

	void foreach_element(std::function<void(size_t i, size_t j, vec_t& v)> fun)
	{
		for (size_t i = 0; i < n; i++)
		{
			for (size_t j = 0; j < m; j++)
			{
				fun(i, j, f[i][j]);
			}
		}
	}

	field_t() : n(0), m(0) {}

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

};

//template<typename T>
struct vec_field_t : field_t<vec_t>
{
	vec_field_t(vec_t size, float spacing) :field_t(size, spacing)
	{
		/*
		std::mt19937 eng(42);
		auto rnd = bind(std::normal_distribution<float>(2 * 3.1415926535897932384626433832795 / 3, 0.2), eng);
		*/

		for (int i = 0; i < n; i++)
		{
			for (int j = 0; j < m; j++)
			{
				float angle = /*rnd()*/0;
				f[i][j] = { cos(angle), sin(angle) };
			}
		}
	}
	vec_field_t(){}
};

//using vec_field_t = field_t < vec_t > ;

using scalar_field_t = field_t < float_t> ;