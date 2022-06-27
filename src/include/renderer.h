#ifndef RENDERER_H
#define RENDERER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include <chip8.h>

extern chip8 _cpu;

void framebuffer_size_callback(GLFWwindow*, int, int);
void key_callback(GLFWwindow*, int, int, int, int);

class context {
private:
	GLFWwindow* glfw_init(int, int, const char*);
public:
	GLFWwindow* window;
	context(int, int, const char*);
};

class viewport {
private:
	int glad_init(const context&, int, int);
public:
	viewport(const context&, int, int);
};

context::context(int window_width, int window_height, const char* window_title)
{
	this->window = glfw_init(window_width, window_height, window_title);
}

GLFWwindow* context::glfw_init(int window_width, int window_height, const char* window_title)
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(window_width, window_height, window_title, NULL, NULL);
	if (!window) {
		std::cerr << "Failed to create GLFW window\n" << std::endl;
		glfwTerminate();
		return NULL;
	}
	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, key_callback);
	return window;
}

viewport::viewport(const context& context, int window_width, int window_height)
{
	glad_init(context, window_width, window_height);
}

int viewport::glad_init(const context& context, int window_width, int window_height)
{
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cerr << "Failed to initialize GLAD" << std::endl;
		return EXIT_FAILURE;
	}
	glViewport(0, 0, window_width, window_height);
	glfwSetFramebufferSizeCallback(context.window, framebuffer_size_callback);
	return EXIT_SUCCESS;
}

void framebuffer_size_callback(GLFWwindow* window, int window_width, int window_height)
{
	glViewport(0, 0, window_width, window_height);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	switch (action) {
	case GLFW_PRESS:
		switch (key) {
		case GLFW_KEY_ESCAPE: glfwSetWindowShouldClose(window, true);	  break;
		case GLFW_KEY_X: _cpu._keypad[0] = 1;							  break;
		case GLFW_KEY_1: _cpu._keypad[1] = 1;							  break;
		case GLFW_KEY_2: _cpu._keypad[2] = 1;							  break;
		case GLFW_KEY_3: _cpu._keypad[3] = 1;							  break;
		case GLFW_KEY_Q: _cpu._keypad[4] = 1;							  break;
		case GLFW_KEY_W: _cpu._keypad[5] = 1;							  break;
		case GLFW_KEY_E: _cpu._keypad[6] = 1;							  break;
		case GLFW_KEY_A: _cpu._keypad[7] = 1;							  break;
		case GLFW_KEY_S: _cpu._keypad[8] = 1;							  break;
		case GLFW_KEY_D: _cpu._keypad[9] = 1;							  break;
		case GLFW_KEY_Z: _cpu._keypad[0xA] = 1;							  break;
		case GLFW_KEY_C: _cpu._keypad[0xB] = 1;							  break;
		case GLFW_KEY_4: _cpu._keypad[0xC] = 1;							  break;
		case GLFW_KEY_R: _cpu._keypad[0xD] = 1;							  break;
		case GLFW_KEY_F: _cpu._keypad[0xE] = 1;							  break;
		case GLFW_KEY_V: _cpu._keypad[0xF] = 1;							  break;
		}
		break;
	case GLFW_RELEASE: memset(_cpu._keypad, 0, sizeof(_cpu._keypad));
		break;
	}
}

#endif // !RENDERER_H