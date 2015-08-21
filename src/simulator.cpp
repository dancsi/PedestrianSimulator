#include <iostream>
#include <cstdio>
#include <random>
#include <chrono>

#include "world.h"
#include "graphics.h"
#include "util.h"
#include "config.h"

using namespace std;

int main(int argc, char** argv)
{
	util::init();
	if (argc > 1)
	{
		config::read(argv[1]);
	}
	else
	{
		config::read("config.ini");
	}
	world::init();
	graphics::init();

	auto start_time = chrono::high_resolution_clock::now();

	world::add_objective({ { 40.5, 20.1 }, 0 });
	//world::add_objective({ { 59, 50 }, 1 });
	atexit(world::destroy);


	while (!graphics::should_exit() && world::people_left && world::step_counter <= world::allowed_steps)
	{
		++world::step_counter;
		world::step(world::timestep);
		graphics::begin_frame();
		world::draw();
		graphics::end_frame();
		//graphics::dump_frame();
	}
	auto end_time = chrono::high_resolution_clock::now();
	chrono::duration<double> elapsed_time = end_time - start_time;

	if (world::step_counter > world::allowed_steps) world::step_counter = INT_MAX;
	printf("%d\n", world::step_counter);
	//fprintf(stderr, "Simulation ended after %f time\n", world::step_counter*world::timestep);
	//fprintf(stderr, "Wall time: %.3f\n", elapsed_time.count());

	//PAUSE();

	return 0;
}