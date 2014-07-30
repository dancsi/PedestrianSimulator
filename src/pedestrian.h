#pragma once 

#include "vec.h"

struct ped_t : public vec_t
{
	float alpha; //orijentacija
	vec_t v, acc;
	ped_t(std::initializer_list<float> params) : vec_t({ *params.begin(), *(params.begin() + 1) }), alpha(*(params.begin() + 2)), v({ 0, 0 }), acc({0, 0}) {}
};