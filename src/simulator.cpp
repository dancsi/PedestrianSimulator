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

	world::add_objective({ { 52, 29 } , 0});
	atexit(world::destroy);

	int step_counter = 0;
	while (!graphics::should_exit() && world::people_left)
	{
		++step_counter;
		graphics::begin_frame();
		world::step(world::timestep);
		world::draw();
		graphics::end_frame();
	}

	printf("Simulation ended after %f time\n", step_counter*world::timestep);
	PAUSE();

	return 0;
}