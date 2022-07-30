#ifndef WINDOW_H
#define WINDOW_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include <chip8.h>

extern chip8 _cpu;

void framebuffer_size_callback(GLFWwindow*, int, int);
void key_callback(GLFWwindow*, int, int, int, int);

class WindowClass {
public:
	GLFWwindow* window;
	WindowClass(int window_width, int window_height, const char* window_title)
	{
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		window = glfwCreateWindow(window_width, window_height, window_title, NULL, NULL);
		if (!window) {
			std::cerr << "Failed to create GLFW window\n" << std::endl;
			glfwTerminate();
			exit(EXIT_FAILURE);
		}
		glfwMakeContextCurrent(window);

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
			std::cerr << "Failed to initialize GLAD" << std::endl;
			exit(EXIT_FAILURE);
		}
		glViewport(0, 0, window_width, window_height);
		glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
		glfwSetKeyCallback(window, key_callback);
	}
};

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

#endif // !WINDOW_H