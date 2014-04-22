#pragma once

#include "vec.h"

#include <functional>

template <typename T, size_t N, size_t M>
struct field_t
{
	const size_t n, m;
	T f[N][M];

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

	field_t() : n(N), m(M) {}

	void operator*=(vec_t::num s)
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

template<size_t N, size_t M>
using vec_field_t = field_t < vec_t, N, M > ;

template<size_t N, size_t M>
using scalar_field_t = field_t < vec_t::num, N, M > ;