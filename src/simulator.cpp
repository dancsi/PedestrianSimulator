#include <iostream>
#include <cstdio>
#include <random>

#include "world.h"
#include "graphics.h"
#include "util.h"
#include "config.h"

using namespace std;

int main(int argc, char** argv)
{
	util::init();
	config::read("config.ini");
	world::init();
	graphics::init();

	mt19937 eng(42);
	uniform_int_distribution<int> dist_w(0, world::width), dist_h(0, world::height);
	auto rnd_w = bind(dist_w, eng);
	auto rnd_h = bind(dist_h, eng);

	bool b = false;

	while (!graphics::should_exit())
	{
		graphics::begin_frame();
		/*
		for (int i = 0; i < 40; i++)
		{
			nvgBeginPath(graphics::vg);
			nvgStrokeWidth(graphics::vg, 1.0/graphics::scale);
			nvgStrokeColor(graphics::vg, nvgRGBA(0, 255, 0, 255)); 
			nvgMoveTo(graphics::vg, rnd_w(), rnd_h());
			nvgLineTo(graphics::vg, rnd_w(), rnd_h());
			nvgStroke(graphics::vg);
		}
		*/

		graphics::draw(world::vf);

		graphics::end_frame();
	}
	
	graphics::destroy();
	return 0;
}