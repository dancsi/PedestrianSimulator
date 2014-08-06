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
	config::read("config.ini");
	util::init();
	world::init();
	graphics::init();

	world::add_objective({ { 102, 26 }, 0});
	world::add_objective({ { 12, 26 }, 1 });
	atexit(world::destroy);


	while (!graphics::should_exit() && world::people_left)
	{
		++world::step_counter;
		graphics::begin_frame();
		world::step(world::timestep);
		world::draw();
		graphics::end_frame();
	}

	printf("Simulation ended after %f time\n", world::step_counter*world::timestep);
	PAUSE();

	return 0;
}