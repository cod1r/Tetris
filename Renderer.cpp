#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <iostream>
#include <fstream>
#include <format>
#include "Tetromino.h"
#include "Tetris.h"
#include "Renderer.h"
Renderer::Renderer()
{
	Renderer::BLOCKSIZE = Renderer::PLAYFIELD_HEIGHT / Tetris::TETRIS_PLAYFIELD_HEIGHT;
	int result = SDL_Init(SDL_INIT_VIDEO);
	if (result < 0) {
		std::cerr << SDL_GetError() << std::endl;
		throw;
	}
	Renderer::WINDOW = SDL_CreateWindow(
		"Tetris",
		0,
		0,
		Renderer::WINDOW_WIDTH,
		Renderer::WINDOW_HEIGHT,
		SDL_WINDOW_OPENGL
	);
	SDL_GLContext ctx = SDL_GL_CreateContext(Renderer::WINDOW);
	if (ctx == NULL) {
		std::cerr << SDL_GetError() << std::endl;
		throw;
	}
	int res = SDL_GL_MakeCurrent(Renderer::WINDOW, ctx);
	if (res < 0) {
		std::cerr << SDL_GetError() << std::endl;
		throw;
	}
	GLenum glew_init_res = glewInit();
	if (glew_init_res != GLEW_OK) {
		std::cerr << "Unable to initialize GLEW" << std::endl;
		std::cerr << glewGetErrorString(glew_init_res) << std::endl;
		throw;
	}
	Renderer::vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	std::ifstream vertex;
	vertex.open("shaders/vertex_shader.glsl");
	if (vertex.is_open()) {
		std::string v_src;
		while (!vertex.eof()) {
			v_src += vertex.get();
		}
		vertex.close();
		int length = v_src.length();
		const GLchar* src = v_src.c_str();
		glShaderSource(Renderer::vertex_shader, 1, &src, &length);
		glCompileShader(Renderer::vertex_shader);
		int status;
		glGetShaderiv(Renderer::vertex_shader, GL_COMPILE_STATUS, &status);
		if (!status) {
			std::cerr << "COMPILATION FAILED" << std::endl;
			int info_log_length;
			glGetShaderiv(Renderer::vertex_shader, GL_INFO_LOG_LENGTH, &info_log_length);
			std::string info_log(info_log_length, ' ');
			glGetShaderInfoLog(Renderer::vertex_shader, info_log.length(), &info_log_length, info_log.data());
			std::cerr << info_log << std::endl;
			throw;
		}
	} else {
		std::cerr << "Shader failed to open" << std::endl;
		throw;
	}
	Renderer::fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	std::ifstream fragment;
	fragment.open("shaders/fragment_shader.glsl");
	if (fragment.is_open()) {
		std::string f_src;
		while (!fragment.eof()) {
			f_src += fragment.get();
		}
		fragment.close();
		int length = f_src.length();
		const GLchar* src = f_src.c_str();
		glShaderSource(Renderer::fragment_shader, 1, &src, &length);
		glCompileShader(Renderer::fragment_shader);
		int status;
		glGetShaderiv(Renderer::fragment_shader, GL_COMPILE_STATUS, &status);
		if (!status) {
			std::cerr << "COMPILATION FAILED" << std::endl;
			int info_log_length;
			glGetShaderiv(Renderer::fragment_shader, GL_INFO_LOG_LENGTH, &info_log_length);
			std::string info_log(info_log_length, ' ');
			glGetShaderInfoLog(Renderer::fragment_shader, info_log.length(), &info_log_length, info_log.data());
			std::cerr << info_log << std::endl;
			throw;
		}
	} else {
		std::cerr << "Fragment Shader failed to open" << std::endl;
		throw;
	}
	Renderer::create_program();
}
std::vector<float> Renderer::convert_coords_to_vertices(Tetromino t)
{
	std::vector<float> vertices{
		t.x1 * Renderer::BLOCKSIZE * 2 / (float)Renderer::PLAYFIELD_WIDTH - 1, - (t.y1 * Renderer::BLOCKSIZE * 2 / (float)Renderer::PLAYFIELD_HEIGHT - 1),
		(t.x1 * Renderer::BLOCKSIZE + Renderer::BLOCKSIZE) * 2 / (float)Renderer::PLAYFIELD_WIDTH - 1, - (t.y1 * Renderer::BLOCKSIZE * 2 / (float)Renderer::PLAYFIELD_HEIGHT - 1),
		t.x1 * Renderer::BLOCKSIZE * 2 / (float)Renderer::PLAYFIELD_WIDTH - 1, - ((t.y1 * Renderer::BLOCKSIZE + Renderer::BLOCKSIZE) * 2 / (float)Renderer::PLAYFIELD_HEIGHT - 1),
		(t.x1 * Renderer::BLOCKSIZE + Renderer::BLOCKSIZE) * 2 / (float)Renderer::PLAYFIELD_WIDTH - 1, - ((t.y1 * Renderer::BLOCKSIZE + Renderer::BLOCKSIZE) * 2 / (float)Renderer::PLAYFIELD_HEIGHT - 1),

		t.x2 * Renderer::BLOCKSIZE * 2 / (float)Renderer::PLAYFIELD_WIDTH - 1, - (t.y2 * Renderer::BLOCKSIZE * 2 / (float)Renderer::PLAYFIELD_HEIGHT - 1),
		(t.x2 * Renderer::BLOCKSIZE + Renderer::BLOCKSIZE) * 2 / (float)Renderer::PLAYFIELD_WIDTH - 1, - (t.y2 * Renderer::BLOCKSIZE * 2 / (float)Renderer::PLAYFIELD_HEIGHT - 1),
		t.x2 * Renderer::BLOCKSIZE * 2 / (float)Renderer::PLAYFIELD_WIDTH - 1, - ((t.y2 * Renderer::BLOCKSIZE + Renderer::BLOCKSIZE) * 2 / (float)Renderer::PLAYFIELD_HEIGHT - 1),
		(t.x2 * Renderer::BLOCKSIZE + Renderer::BLOCKSIZE) * 2 / (float)Renderer::PLAYFIELD_WIDTH - 1, - ((t.y2 * Renderer::BLOCKSIZE + Renderer::BLOCKSIZE) * 2 / (float)Renderer::PLAYFIELD_HEIGHT - 1),

		t.x3 * Renderer::BLOCKSIZE * 2 / (float)Renderer::PLAYFIELD_WIDTH - 1, - (t.y3 * Renderer::BLOCKSIZE * 2 / (float)Renderer::PLAYFIELD_HEIGHT - 1),
		(t.x3 * Renderer::BLOCKSIZE + Renderer::BLOCKSIZE) * 2 / (float)Renderer::PLAYFIELD_WIDTH - 1, - (t.y3 * Renderer::BLOCKSIZE * 2 / (float)Renderer::PLAYFIELD_HEIGHT - 1),
		t.x3 * Renderer::BLOCKSIZE * 2 / (float)Renderer::PLAYFIELD_WIDTH - 1, - ((t.y3 * Renderer::BLOCKSIZE + Renderer::BLOCKSIZE) * 2 / (float)Renderer::PLAYFIELD_HEIGHT - 1),
		(t.x3 * Renderer::BLOCKSIZE + Renderer::BLOCKSIZE) * 2 / (float)Renderer::PLAYFIELD_WIDTH - 1, - ((t.y3 * Renderer::BLOCKSIZE + Renderer::BLOCKSIZE) * 2 / (float)Renderer::PLAYFIELD_HEIGHT - 1),

		t.x4 * Renderer::BLOCKSIZE * 2 / (float)Renderer::PLAYFIELD_WIDTH - 1, - (t.y4 * Renderer::BLOCKSIZE * 2 / (float)Renderer::PLAYFIELD_HEIGHT - 1),
		(t.x4 * Renderer::BLOCKSIZE + Renderer::BLOCKSIZE) * 2 / (float)Renderer::PLAYFIELD_WIDTH - 1, - (t.y4 * Renderer::BLOCKSIZE * 2 / (float)Renderer::PLAYFIELD_HEIGHT - 1),
		t.x4 * Renderer::BLOCKSIZE * 2 / (float)Renderer::PLAYFIELD_WIDTH - 1, - ((t.y4 * Renderer::BLOCKSIZE + Renderer::BLOCKSIZE) * 2 / (float)Renderer::PLAYFIELD_HEIGHT - 1),
		(t.x4 * Renderer::BLOCKSIZE + Renderer::BLOCKSIZE) * 2 / (float)Renderer::PLAYFIELD_WIDTH - 1, - ((t.y4 * Renderer::BLOCKSIZE + Renderer::BLOCKSIZE) * 2 / (float)Renderer::PLAYFIELD_HEIGHT - 1),
	};
	return vertices;
}
void Renderer::render_tetromino(Tetromino t)
{
	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	std::vector<float> vertices = Renderer::convert_coords_to_vertices(t);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), vertices.data(), GL_DYNAMIC_DRAW);
	GLuint location = glGetAttribLocation(Renderer::current_program, "pos");
	if (location == -1) {
		std::cerr << "pos's location could not be found" << std::endl;
		throw;
	}
	glVertexAttribPointer(location, 2, GL_FLOAT, false, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(location);

	if (glIsBuffer(Renderer::current_vbo)) {
		Renderer::vertex_buffer_objects.push_back(Renderer::current_vbo);
		Renderer::create_program();
	}
	GLint out_color_location = glGetUniformLocation(Renderer::current_program, "out_color");
	if (out_color_location == -1) {
		std::cerr << "out_color's location could not be found" << std::endl;
		throw;
	}
	glUseProgram(Renderer::current_program);
	glUniform4f(out_color_location, t.red/255.0f, t.green/255.0f, t.blue/255.0f, 1.0f);
	Renderer::current_vbo = vbo;
}
void Renderer::create_program()
{
	if (glIsProgram(Renderer::current_program))
		Renderer::programs.push_back(Renderer::current_program);
	Renderer::current_program = glCreateProgram();
	glAttachShader(Renderer::current_program, Renderer::vertex_shader);
	glAttachShader(Renderer::current_program, Renderer::fragment_shader);
	glLinkProgram(Renderer::current_program);
}
void Renderer::update_tetromino(Tetromino t)
{
	glBindBuffer(GL_ARRAY_BUFFER, Renderer::current_vbo);
	std::vector<float> vertices = Renderer::convert_coords_to_vertices(t);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * vertices.size(), vertices.data());
}
void Renderer::render()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glUseProgram(Renderer::current_program);
	glBindBuffer(GL_ARRAY_BUFFER, Renderer::current_vbo);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 16);
	for (int idx = 0; idx < Renderer::vertex_buffer_objects.size(); ++idx) {
		glUseProgram(Renderer::programs.at(idx));
		glBindBuffer(GL_ARRAY_BUFFER, Renderer::vertex_buffer_objects.at(idx));
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 16);
	}
	SDL_GL_SwapWindow(Renderer::WINDOW);
}
Renderer::~Renderer()
{
	SDL_DestroyWindow(Renderer::WINDOW);
	SDL_Quit();
}
