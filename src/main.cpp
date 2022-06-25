#include <iostream>
#include <string>

#include "chip8.h"
#include "renderer.h"
#include "shader.h"

const unsigned int VIDEO_WIDTH = 64;
const unsigned int VIDEO_HEIGHT = 32;
const char *WINDOW_TITLE = "CHIP8 emu";

int main(int argc, char* argv[])
{
	Shader shader("vertex_shader.glsl", "fragment_shader.glsl");

	float vertices[] = {
		 1.0f,  1.0f, 0.0f,   1.0f, 1.0f, // top right
		 1.0f, -1.0f, 0.0f,   1.0f, 0.0f, // bottom right
		-1.0f, -1.0f, 0.0f,   0.0f, 0.0f, // bottom left
		-1.0f,  1.0f, 0.0f,   0.0f, 1.0f  // top left 
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

	if (argc != 4) {
		std::cerr << "Usage: <path> <scale> <delay> <ROM>" << std::endl;
		std::exit(EXIT_FAILURE);
	}

	int video_scale = std::stoi(argv[1]);
	int cycle_delay = std::stoi(argv[2]);
	char const* rom_file_name = argv[3];

	chip8 Chip8;
	Chip8.load_rom(rom_file_name);

	int video_pitch = sizeof(Chip8._video[0]) * VIDEO_WIDTH;

	context context(VIDEO_WIDTH, VIDEO_HEIGHT, WINDOW_TITLE);
	viewport viewport(context, VIDEO_WIDTH, VIDEO_HEIGHT);

	bool quit = false;
	auto last_cycle_time = std::chrono::high_resolution_clock::now();

	while (!glfwWindowShouldClose(context.window)) {
		// Render loop
		process_input(context.window, Chip8._keypad);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glBindTexture(GL_TEXTURE_2D, texture);
		
		auto current_time = std::chrono::high_resolution_clock::now();
		float dt = std::chrono::duration<float, std::chrono::milliseconds::period>(current_time - last_cycle_time).count();

		if (dt > cycle_delay) {
			last_cycle_time = current_time;
			Chip8.cycle();
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, VIDEO_WIDTH * video_scale, VIDEO_HEIGHT * video_scale, 0, GL_RGBA, GL_UNSIGNED_BYTE, Chip8._video);
		}

		shader.use();
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(context.window);
		glfwPollEvents();
	}
	glfwTerminate();
}