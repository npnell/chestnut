#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
public:
	unsigned ID;

	Shader(const char* vertex_path, const char* fragment_path) {
		std::string vertex_code;
		std::string fragment_code;
		std::ifstream v_shader_file;
		std::ifstream f_shader_file;

		v_shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		f_shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		try {
			v_shader_file.open(vertex_path);
			f_shader_file.open(fragment_path);
			std::stringstream v_shaderstream, f_shaderstream;
			v_shaderstream << v_shader_file.rdbuf();
			f_shaderstream << f_shader_file.rdbuf();

			vertex_code = v_shaderstream.str();
			fragment_code = f_shaderstream.str();
		}
		catch (std::ifstream::failure& e) {
			std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << std::endl;
		}
		const char* v_shader_code = vertex_code.c_str();
		const char* f_shader_code = fragment_code.c_str();

		unsigned int vertex, fragment;
		// Vertex shader
		vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &v_shader_code, NULL);
		glCompileShader(vertex);
		check_compile_errors(vertex, "VERTEX");
		// Fragment shader
		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &f_shader_code, NULL);
		glCompileShader(fragment);
		check_compile_errors(fragment, "FRAGMENT");
		// Shader Program
		ID = glCreateProgram();
		glAttachShader(ID, vertex);
		glAttachShader(ID, fragment);
		glLinkProgram(ID);
		check_compile_errors(ID, "PROGRAM");

		glDeleteShader(vertex);
		glDeleteShader(fragment);
	}

	inline void use() const;

	inline void set_bool(const std::string& name, bool value) const;

	inline void set_int(const std::string& name, int value) const;

	inline void set_float(const std::string& name, float value) const;

private:
	void check_compile_errors(GLuint shader, std::string type);
};

// activate the shader
// ------------------------------------------------------------------------
inline void Shader::use() const
{
    glUseProgram(ID);
}
// utility uniform functions
// ------------------------------------------------------------------------
inline void Shader::set_bool(const std::string& name, bool value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}
// ------------------------------------------------------------------------
inline void Shader::set_int(const std::string& name, int value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}
// ------------------------------------------------------------------------
inline void Shader::set_float(const std::string& name, float value) const
{
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}
// ------------------------------------------------------------------------

void Shader::check_compile_errors(GLuint shader, std::string type)
{
	GLint success;
	GLchar infoLog[1024];
	if (type != "PROGRAM")
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERROR::SHADER_COMPILATION_ERROR::" << type << "\n" << infoLog << std::endl;
		}
	}
	else
	{
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERROR::PROGRAM_LINKING_ERROR::" << type << "\n" << infoLog << std::endl;
		}
	}
}

#endif // !SHADER_H
