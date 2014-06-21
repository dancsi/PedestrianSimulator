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

	while (!graphics::should_exit())
	{
		graphics::begin_frame();
		world::draw();
		graphics::end_frame();
	}
	
	graphics::destroy();
	return 0;
}