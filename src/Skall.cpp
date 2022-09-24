#include <iostream>
#include <time.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <assert.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Wrappers.hpp"
#include "Defines.h"
#include "Loader.hpp"
#include "GLDebugContext.hpp"
#include "Mesh.hpp"
#include "Window.hpp"
#include "Camera.hpp"
#include "InterfaceMap.hpp"
#include "Material.hpp"
#include "Renderer.hpp"
using namespace std;
using namespace glm;

GLFWwindow* createWindow();
void configureContex();

// todo flashlight, world ambient

void run()
{
	LOG("Initializing GLFW");
	assert(glfwInit());

	LOG("Creating window");
	GLFWwindow* window = createWindow();
	Window::SetWindow(window);

	LOG("Loading OpenGL functions");
#ifdef GLAD_GL_H_
	assert(gladLoadGL(glfwGetProcAddress));
#else
	assert(gladLoadGL());
#endif
	if (!GLAD_GL_ARB_bindless_texture)
		FAIL("GPU unsupported (GL_ARB_bindless_texture)");

	LOG("Configuring context")
	configureContex();

	Renderer renderer();

	LOG("Initialization complete\n");
	Window::CenterCursor();
	while (!glfwWindowShouldClose(window)) {
		Window::NewFrame();
		{ // INPUT
			glfwPollEvents();
			if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
				glfwSetWindowShouldClose(window, true);
			player.ProcessInput();
		}
		renderer.Render();
	}
	LOG("\n");
}

void configureContex()
{
	glfwSwapInterval(0);
	glfwSetInputMode(Window::Window(), GLFW_STICKY_KEYS, GL_TRUE);
	glfwSetInputMode(Window::Window(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	if (DBGOUT)
		enableDebug();
	glEnable(GL_BLEND);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_MULTISAMPLE);
	glClearColor(0, 0, 0, 1);
}

GLFWwindow* createWindow()
{
	if (DBGOUT)
		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	if (Settings::Fullscreen()) {
		GLFWmonitor* monitor = glfwGetPrimaryMonitor();
		const GLFWvidmode* mode = glfwGetVideoMode(monitor);
		Settings::SetResolution(mode->width, mode->height);
	}
	GLFWwindow* window = glfwCreateWindow(Settings::Width(), Settings::Height(), "CppCraft",
					      Settings::Fullscreen() ? glfwGetPrimaryMonitor() : NULL, NULL);
	assert(window);
	glfwMakeContextCurrent(window);
	return window;
}

int main()
{
	srand(time(0));
	ios_base::sync_with_stdio(false);
	cin.tie(nullptr);
	cout << endl;
	run();
	cout << flush;
	return 0;
}
