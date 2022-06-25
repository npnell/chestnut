#ifndef RENDERER_H
#define RENDERER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

void framebuffer_size_callback(GLFWwindow*, int, int);
void process_input(GLFWwindow*, uint8_t*);

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

void process_input(GLFWwindow* window, uint8_t* key)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
		key[0] = 1;
	else key[0] = 0;

	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
		key[1] = 1;
	else key[1] = 0;

	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
		key[2] = 1;
	else key[2] = 0;

	if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
		key[3] = 1;
	else key[3] = 0;

	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		key[4] = 1;
	else key[4] = 0;

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		key[5] = 1;
	else key[5] = 0;

	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		key[6] = 1;
	else key[6] = 0;

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		key[7] = 1;
	else key[7] = 0;

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		key[8] = 1;
	else key[8] = 0;

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		key[9] = 1;
	else key[9] = 0;

	if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
		key[0xA] = 1;
	else key[0xA] = 0;

	if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
		key[0xB] = 1;
	else key[0xB] = 0;

	if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
		key[0xC] = 1;
	else key[0xC] = 0;

	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
		key[0xD] = 1;
	else key[0xD] = 0;

	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
		key[0xE] = 1;
	else key[0xE] = 0;

	if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS)
		key[0xF] = 1;
	else key[0xF] = 0;
}

#endif // !RENDERER_H
