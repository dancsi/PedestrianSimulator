#include "world.h"
#include "config.h"
#include "util.h"

namespace world
{
	int width, height;
	void init()
	{
		width = config::get<int>("world", "width");
		height = config::get<int>("world", "height");
		LOG("world size (%d, %d)", width, height);
	}
}