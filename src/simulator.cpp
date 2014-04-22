#include <iostream>
#include <cstdio>

#include "field.h"

using namespace std;

vec_field_t<512, 512> vf;

int main(int argc, char** argv)
{
	vf.foreach_element([&](int i, int j, vec_t& el) { printf("%.2f\n", el.x); });


	return 0;
}