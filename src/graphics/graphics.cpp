#include "graphics.h"
#include "util.h"
#include "vec.h"
#include "world.h"
#include "field.h"
#include "geometry.h"

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
	float one_pixel = 1.0;

	NVGcontext* vg = NULL;
	GPUtimer gpu_timer;
	PerfGraph fps_graph, cpu_graph, gpu_graph;
	double prev_time = 0, cpu_time = 0, render_start_time, delta_time = 1./60.0;

	vecd_t mouse_pos;

	void init()
	{
#ifdef GRAPHICS
		if (!glfwInit()) {
			LOG("Failed to init GLFW.");
			exit(-1);
		}

		initGraph(&fps_graph, GRAPH_RENDER_FPS, "Frame Time");
		initGraph(&cpu_graph, GRAPH_RENDER_MS, "CPU Time");
		initGraph(&gpu_graph, GRAPH_RENDER_MS, "GPU Time");

		glfwSetErrorCallback([](int error, const char* desc) {LOG("GLFW error %d: %s\n", error, desc); });
		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, 1);
		int height = glfwGetVideoMode(glfwGetPrimaryMonitor())->height*0.6, width = height*double(world::width) / double(world::height);
		window = glfwCreateWindow(width, height, "PedestrianSimulator", NULL, NULL); if (!window) {
			glfwTerminate();
			exit(-1);
		}
		glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			NVG_NOTUSED(scancode);
			NVG_NOTUSED(mods);
			if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
				glfwSetWindowShouldClose(window, GL_TRUE);
		});

		glfwSetMouseButtonCallback(window, [](GLFWwindow* wind, int button, int action, int mods){
			static vec_t start_pos;
			vecd_t pos;
			glfwGetCursorPos(wind, &pos.x, &pos.y);
			vec_t world_pos = screen2world({ pos.x, pos.y });
			if (action == GLFW_RELEASE)
			{
				
				if (button == GLFW_MOUSE_BUTTON_1)
				{
					world::add_objective({ world_pos, 0 });
				}
				else if (button == GLFW_MOUSE_BUTTON_2)
				{
					world::add_objective({ world_pos, 1 });
				}
				else if (button==GLFW_MOUSE_BUTTON_3)
					 LOG("click at screen (%.2lf, %.2lf), world (%.2f, %.2f)", pos.x, pos.y, world_pos.x, world_pos.y);
			}
			else if (action == GLFW_PRESS)
			{
				if (button == GLFW_MOUSE_BUTTON_2)
				{
					start_pos = world_pos;
				}
			}
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
#endif
	}

	GLuint rbo_id[2], fbo_id[2];


	bool should_exit()
	{
#ifdef GRAPHICS
		return glfwWindowShouldClose(window);
#else
		return false;
#endif
	}

	void begin_frame()
	{
#ifdef GRAPHICS
		render_start_time = glfwGetTime();
		delta_time = render_start_time - prev_time;
		prev_time = render_start_time;

		startGPUTimer(&gpu_timer);
		glfwGetCursorPos(window, &mouse_pos.x, &mouse_pos.y);
		glfwGetWindowSize(window, &window_width, &window_height);
		glfwGetFramebufferSize(window, &fb_width, &fb_height);
		glViewport(0, 0, fb_width, fb_height);
		glClearColor(0.3, 0.3, 0.32, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		nvgBeginFrame(vg, fb_width, fb_height, 1);
		draw_world_scale();
#endif
	}

	void end_frame()
	{
#ifdef GRAPHICS
		draw_ui_scale();
		renderGraph(vg, 5, 5, &fps_graph);
		renderGraph(vg, 5 + 200 + 5, 5, &cpu_graph);
		if (gpu_timer.supported)
			renderGraph(vg, 5 + 200 + 5 + 200 + 5, 5, &gpu_graph);
		draw_world_scale();
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
#endif
	}

	void destroy()
	{
#ifdef GRAPHICS
		nvgDeleteGL3(vg);
		glfwTerminate();
#endif
	}

	void draw_ui_scale()
	{
#ifdef GRAPHICS
		nvgResetTransform(vg);
		one_pixel = 1.0;
#endif
	}

	void draw_world_scale()
	{
#ifdef GRAPHICS
		nvgResetTransform(vg);
		nvgTranslate(vg, fb_width / 2, fb_height / 2);
		nvgScale(vg, double(fb_width) / world::width, double(fb_height) / world::height);
		nvgTranslate(vg, -world::width / 2, -world::height / 2);
		one_pixel = double(world::width) / fb_width;
#endif
	}

#ifdef GRAPHICS
	template<>
	void draw<vec_field_t>(vec_field_t& vf, NVGcolor& color)
	{
		vf.foreach_element([&](size_t i, size_t j, vec_t& v){
			vec_t pos = vf.get_coordinates(i, j), to = pos + v;

			nvgBeginPath(vg);
			nvgCircle(vg, pos.x, pos.y, .2);
			nvgFillColor(vg, color);
			nvgFill(vg);

			nvgBeginPath(vg);
			nvgMoveTo(vg, pos.x, pos.y);
			nvgLineTo(vg, to.x, to.y);
			nvgStrokeColor(vg, color);
			nvgStrokeWidth(vg, one_pixel);
			nvgStroke(vg);
		});
	}

	template<>
	void draw<circle_t>(circle_t& circle, NVGcolor& color)
	{
		nvgBeginPath(graphics::vg);
		nvgCircle(graphics::vg, circle.x, circle.y, circle.r);
		nvgFillColor(graphics::vg, color);
		nvgFill(graphics::vg);
	}

	template<>
	void draw<line_t>(line_t& line, NVGcolor& color)
	{
		nvgBeginPath(graphics::vg);
		nvgMoveTo(graphics::vg, line.p.x, line.p.y);
		nvgLineTo(graphics::vg, line.q.x, line.q.y);
		nvgStrokeColor(graphics::vg, color);
		nvgStrokeWidth(graphics::vg, 3 * graphics::one_pixel);
		nvgStroke(graphics::vg);
	}
#endif

	vec_t world2screen(vec_t v)
	{
		float transform[6];
		nvgCurrentTransform(vg, transform);
		vec_t ret;
		nvgTransformPoint(&ret.x, &ret.y, transform, v.x, v.y);
		return ret;
	}

	vec_t screen2world(vec_t v)
	{
		float transform[6], itransform[6];
		nvgCurrentTransform(vg, transform);
		nvgTransformInverse(itransform, transform);
		vec_t ret;
		nvgTransformPoint(&ret.x, &ret.y, itransform, v.x, v.y);
		return ret;
	}

}


