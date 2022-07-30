#include <iostream>
#include <thread>

#include <chip8.h>
#include <window.h>
#include <shader.h>

#define FRAME_INTERVAL 1.0f / 60
#define MULTIPLIER 4

chip8 _cpu;

const unsigned int WINDOW_WIDTH = 640;
const unsigned int WINDOW_HEIGHT = 320;
const char *WINDOW_TITLE = "CHIP8 emu";

int main(int argc, char* argv[])
{
	if (argc != 2) {
		std::cerr << "Usage: <ROM>" << std::endl;
		std::exit(EXIT_FAILURE);
	}

	char const* rom_file_name = argv[1];

	_cpu.load_rom(rom_file_name);

	WindowClass window(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE);

	Shader shader("shaders/vertex_shader.glsl", "shaders/fragment_shader.glsl");

	float vertices[] = {
		 1.0f,  1.0f, 0.0f,   1.0f, 1.0f, // top right
		 1.0f, -1.0f, 0.0f,   1.0f, 0.0f, // bottom right
		-1.0f, -1.0f, 0.0f,   0.0f, 0.0f, // bottom left

		-1.0f, -1.0f, 0.0f,   0.0f, 0.0f, // bottom left
		-1.0f,  1.0f, 0.0f,   0.0f, 1.0f,  // top left
		 1.0f,  1.0f, 0.0f,   1.0f, 1.0f, // top right
	};

	unsigned int VAO, VBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// texture attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	double current_time, last_time = glfwGetTime();

	while (!glfwWindowShouldClose(window.window)) {
		// Render loop
		current_time = glfwGetTime();
		
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		if (current_time * MULTIPLIER - last_time >= FRAME_INTERVAL) {
			last_time = current_time;
			_cpu.cycle();
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 64, 32, 0, GL_RGBA, GL_UNSIGNED_BYTE, _cpu._video);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		shader.use();
		glBindVertexArray(VAO);
		glBindTexture(GL_TEXTURE_2D, texture);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		glfwSwapBuffers(window.window);
		glfwPollEvents();
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteTextures(1, &texture);

	glfwTerminate();
}