#include "graphics.h"
#include "util.h"

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

using namespace graphics;

void graphics::init()
{
	if (!glfwInit())
	{
		LOG("failed to init graphics");
		exit(-1);
	}
}
