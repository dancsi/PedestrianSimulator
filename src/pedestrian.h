#pragma once 

#include "vec.h"

static int id_counter = 0;

namespace ped_parameters
{
	const static float preferred_velocity = 1.5;
	const static float maximum_velocity = 3;
	const static float relaxation_time = 0.5;
}

struct ped_t : public vec_t
{
	float alpha; //orijentacija
	vec_t v, acc;
	int id;
	bool arrived_at_destination;
	ped_t(std::initializer_list<float> params) : vec_t({ *params.begin(), *(params.begin() + 1) }), alpha(*(params.begin() + 2)), v({ 0, 0 }), acc({0, 0}), arrived_at_destination(0), id(id_counter++) {}
	ped_t& operator=(vec_t vec)
	{
		x = vec.x;
		y = vec.y;
		return *this;
	}
	bool operator==(ped_t& other)
	{
		return id == other.id;
	}
	bool operator!=(ped_t& other)
	{
		return id != other.id;
	}
};