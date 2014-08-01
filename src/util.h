#pragma once

#include <ctime>
#include <cstdlib>
#include <cstdio>
#include <functional>

#define LOG(format, ...) do {fprintf(stderr, "%s(%d): " format, __FILE__, __LINE__, __VA_ARGS__); fprintf(stderr, "\n");} while(0)

struct pair_t
{
	size_t i, j;
	bool operator==(pair_t p)
	{
		return i == p.i&&j == p.j;
	}
};


namespace util
{
	void init();
	float rand_range(float low, float high);
	inline float sqr(float x) { return x*x; }

	template<typename T>
	T** alloc_matrix(size_t n, size_t m)
	{
		T* tmp = new T[n*m];
		T** f = new T*[n];
		for (size_t i = 0; i < n; i++) f[i] = tmp + i*m;
		return f;
	}

	template<typename T>
	struct matrix_t
	{
		size_t n, m;
		T** f;
		matrix_t() :n(0), m(0), f(nullptr) {}
		matrix_t(size_t n, size_t m) :n(n), m(m)
		{
			f = alloc_matrix<T>(n, m);
		}
		inline T& operator[](pair_t& idx)
		{
			return f[idx.i][idx.j];
		}
		void clear()
		{
			memset((void*) (*f), 0, n*m*sizeof(T));
		}

		void foreach_row(std::function<void(size_t i, T*& row)> fun)
		{
			for (size_t i = 0; i < n; i++) fun(i, f[i]);
		}

		void foreach_element(std::function<void(size_t i, size_t j, T& v)> fun)
		{
			for (size_t i = 0; i < n; i++)
			{
				for (size_t j = 0; j < m; j++)
				{
					fun(i, j, f[i][j]);
				}
			}
		}
		void set(T x)
		{
			foreach_element([&x](int i, int j, T& v) {v = x; });
		}
	};
}

