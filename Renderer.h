#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <vector>
#ifndef _RENDERER_H
#define _RENDERER_H
struct Renderer {
	SDL_Window* WINDOW = nullptr;
	const int32_t WINDOW_WIDTH = 300;
	const int32_t WINDOW_HEIGHT = 600;
	const int32_t PLAYFIELD_WIDTH = 300;
	const int32_t PLAYFIELD_HEIGHT = 600;
	int32_t BLOCKSIZE;
	GLuint vertex_shader;
	GLuint fragment_shader;
	GLuint current_vbo;
	GLuint current_program;
	std::vector<GLuint> programs;
	std::vector<GLuint> vertex_buffer_objects;
	void render_tetromino(Tetromino);
	void update_tetromino(Tetromino);
	void render();
	void create_program();
	Renderer();
	~Renderer();
};
#endif
