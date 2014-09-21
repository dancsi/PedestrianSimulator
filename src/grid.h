#pragma once

#include <vector>

#include "util.h"
#include "pedestrian.h"

using namespace std;
using namespace util;

struct grid_t
{
	vector<ped_t> people;
	matrix_t<vector<ped_t>::iterator> cells;
	
	inline void push_back(ped_t& p)
	{
		people.push_back(p);
	}
	inline size_t size()
	{
		return people.size();
	}
	inline vector<ped_t>::iterator begin()
	{
		return people.begin();
	}
	inline vector<ped_t>::iterator end()
	{
		return people.end();
	}
};