#pragma once

#include <nanovg.h>

namespace graphics
{
	extern NVGcontext* vg;
	extern int height, width;

	void init();
	bool should_exit();
	void begin_frame();
	void end_frame();
	void destroy();
}