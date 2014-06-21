#pragma once

#include "vec.h"

#include <nanovg.h>
#include <functional>

namespace graphics
{
	extern NVGcontext* vg;
	extern float one_pixel;

	void init();
	bool should_exit();
	void begin_frame();
	void end_frame();
	void destroy();
	void draw_ui_scale();
	void draw_world_scale();
	vec_t world2screen(vec_t v);
	vec_t screen2world(vec_t v);

	template<typename T>
	void draw(T& drawable, NVGcolor& color = nvgRGB(0, 0, 0));
}