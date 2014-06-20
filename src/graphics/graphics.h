#pragma once

#include <nanovg.h>

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
}