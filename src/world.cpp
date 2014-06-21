#include "world.h"
#include "config.h"
#include "util.h"
#include "vec.h"

namespace world
{
	int width, height;
	vec_field_t vf;
	void init()
	{
		width = config::get<int>("world", "width");
		height = config::get<int>("world", "height");
		float spacing = config::get<double>("world", "spacing");
		vf = vec_field_t({width, height}, spacing);
	}
}