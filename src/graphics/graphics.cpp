#include "graphics.h"
#include "util.h"
#include "vec.h"

#define NANOVG_GLEW
#define GLEW_STATIC

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "nanovg.h"
#define NANOVG_GL3_IMPLEMENTATION
#include "nanovg_gl.h"
#include "perf.h"

namespace graphics
{
	GLFWwindow* window;
	int window_width, window_height;
	int fb_width, fb_height;
	int width, height;
	const int canvas_scale = 100;

	NVGcontext* vg = NULL;
	GPUtimer gpu_timer;
	PerfGraph fps_graph, cpu_graph, gpu_graph;
	double prev_time=0, cpu_time=0, render_start_time, delta_time;

	vecd_t mouse_pos;

	void init()
	{
		if (!glfwInit()) {
			LOG("Failed to init GLFW.");
			exit(-1);
		}

		initGraph(&fps_graph, GRAPH_RENDER_FPS, "Frame Time");
		initGraph(&cpu_graph, GRAPH_RENDER_MS, "CPU Time");
		initGraph(&gpu_graph, GRAPH_RENDER_MS, "GPU Time");

		glfwSetErrorCallback([&](int error, const char* desc) {LOG("GLFW error %d: %s\n", error, desc); });
		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, 1);
		window = glfwCreateWindow(800, 600, "PedestrianSimulator", NULL, NULL); if (!window) {
			glfwTerminate();
			exit(-1);
		}
		glfwSetKeyCallback(window, [&](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			NVG_NOTUSED(scancode);
			NVG_NOTUSED(mods);
			if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
				glfwSetWindowShouldClose(window, GL_TRUE);
		});
		glfwMakeContextCurrent(window);
		glewExperimental = GL_TRUE;
		if (glewInit() != GLEW_OK) {
			LOG("Could not init glew.\n");
			exit(-1);
		}
		glGetError();
		vg = nvgCreateGL3(512, 512, NVG_ANTIALIAS | NVG_STENCIL_STROKES);
		if (vg == NULL) {
			LOG("Could not init nanovg.\n");
			exit(-1);
		}

		nvgCreateFont(vg, "sans", "roboto.ttf");

		glfwSwapInterval(0);
		initGPUTimer(&gpu_timer);
		glfwSetTime(0);
		prev_time = glfwGetTime();
	}

	bool should_exit()
	{
		return glfwWindowShouldClose(window);
	}

	void begin_frame()
	{
		render_start_time = glfwGetTime();
		delta_time = render_start_time - prev_time;
		prev_time = render_start_time;

		startGPUTimer(&gpu_timer);
		glfwGetCursorPos(window, &mouse_pos.x, &mouse_pos.y);
		glfwGetWindowSize(window, &window_width, &window_height);
		glfwGetFramebufferSize(window, &fb_width, &fb_height);
		width = canvas_scale * double(window_width) / double(window_height); height = canvas_scale;
		glViewport(0, 0, width, height);
		glClearColor(0.3, 0.3, 0.32, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		nvgBeginFrame(vg, width, height, /*double(fb_width)/double(window_width)*/1.0);
		
	}

	void end_frame()
	{
		renderGraph(vg, 5, 5, &fps_graph);
		renderGraph(vg, 5 + 200 + 5, 5, &cpu_graph);
		if (gpu_timer.supported)
			renderGraph(vg, 5 + 200 + 5 + 200 + 5, 5, &gpu_graph);

		nvgEndFrame(vg);

		cpu_time = glfwGetTime() - render_start_time;

		updateGraph(&fps_graph, delta_time);
		updateGraph(&cpu_graph, cpu_time);

		float gpu_times[3];
		int n = stopGPUTimer(&gpu_timer, gpu_times, 3);
		for (int i = 0; i < n; i++)
			updateGraph(&gpu_graph, gpu_times[i]);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	void destroy()
	{
		nvgDeleteGL3(vg);
		glfwTerminate();
	}

}


