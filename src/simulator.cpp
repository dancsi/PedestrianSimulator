#include <iostream>
#include <cstdio>
#include <random>

#include "field.h"
#include "graphics/graphics.h"
#include "util.h"

using namespace std;

vec_field_t<512, 512> vf;

int main(int argc, char** argv)
{
	util::init();
	graphics::init();

	mt19937 eng(42);
	uniform_int_distribution<int> dist(0, graphics::height);
	auto rnd = bind(dist, eng);

	while (!graphics::should_exit())
	{
		graphics::begin_frame();

		for (int i = 0; i < 4000; i++)
		{
			nvgBeginPath(graphics::vg);
			nvgStrokeColor(graphics::vg, nvgRGBA(0, 255, 0, 255));
			nvgMoveTo(graphics::vg,rnd(), rnd());
			nvgLineTo(graphics::vg, rnd(), rnd());
			nvgStroke(graphics::vg);
		}

		graphics::end_frame();
	}
	
	graphics::destroy();
	return 0;
}