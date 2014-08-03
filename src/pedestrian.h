#pragma once 

#include "vec.h"

static int id_counter = 0;

namespace ped_parameters
{
	static float preferred_velocity = 5;
	static float maximum_velocity = 5;
	static float relaxation_time = 0.5;
	static float fov_half_angle = 100.0 / 180.0 * M_PI;
	static float granular_interactions_radius = 2;
	static float body_force_k = 100;
	static float sliding_friction_k = 100;
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