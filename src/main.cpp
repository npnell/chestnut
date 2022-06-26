#include <iostream>
#include <string>

#include <chip8.h>
#include <renderer.h>
#include <shader.h>

const unsigned int WINDOW_WIDTH = 640;
const unsigned int WINDOW_HEIGHT = 320;
const char *WINDOW_TITLE = "CHIP8 emu";

int main(int argc, char* argv[])
{
	if (argc != 3) {
		std::cerr << "Usage: <delay> <ROM>" << std::endl;
		std::exit(EXIT_FAILURE);
	}

	float cycle_delay = std::stof(argv[1]);
	char const* rom_file_name = argv[2];

	chip8 Chip8;
	Chip8.load_rom(rom_file_name);

	context context(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE);
	viewport viewport(context, WINDOW_WIDTH, WINDOW_HEIGHT);

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

	bool quit = false;
	auto last_cycle_time = std::chrono::high_resolution_clock::now();

	while (!glfwWindowShouldClose(context.window)) {
		// Render loop
		process_input(context.window, Chip8._keypad);

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		
		auto current_time = std::chrono::high_resolution_clock::now();
		float dt = std::chrono::duration<float, std::chrono::milliseconds::period>(current_time - last_cycle_time).count();

		if (dt > cycle_delay) {
			last_cycle_time = current_time;
			Chip8.cycle();
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 64, 32, 0, GL_RGBA, GL_UNSIGNED_BYTE, Chip8._video);
			glGenerateMipmap(GL_TEXTURE_2D);
		}

		shader.use();
		glBindVertexArray(VAO);
		glBindTexture(GL_TEXTURE_2D, texture);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		glfwSwapBuffers(context.window);
		glfwPollEvents();
	}
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	glfwTerminate();
}