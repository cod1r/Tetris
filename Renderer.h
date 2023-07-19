#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <vector>
#ifndef _RENDERER_H
#define _RENDERER_H
#include "Tetris.h"
#include "Tetromino.h"
struct Renderer {
	SDL_Window* WINDOW = nullptr;
	static const int32_t HEIGHT = 600;
	const int32_t WIDTH = 300;
	const static int32_t BLOCKSIZE = HEIGHT / TETRIS_PLAYFIELD_HEIGHT;
	GLuint vertex_shader;
	GLuint fragment_shader;
	GLuint current_vbo;
	GLuint current_program;
	std::vector<GLuint> programs;
	std::vector<GLuint> vertex_buffer_objects;
	void render_tetromino(Tetromino);
	void update_tetromino();
	void render();
	void create_program();
	Renderer();
	~Renderer();
};
#endif
