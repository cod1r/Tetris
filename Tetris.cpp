#include <iostream>
#include <vector>
#include <SDL2/SDL.h>
#include <stdexcept>
#include "Tetris.h"

Tetris::Tetris(int width, int height)
{
	Tetris::playfield = \
		std::vector<std::vector<int>>(10, std::vector<int>(20, 0));
	int result = SDL_Init(SDL_INIT_VIDEO);
	if (result < 0) {
		throw std::runtime_error(SDL_GetError());
	}
	Tetris::window = SDL_CreateWindow(
		"Tetris",
		0,
		0,
		width,
		height,
		0
	);
	Tetris::loop();
}

void Tetris::loop()
{
	SDL_Event e;
	bool quit = false;
	while (!quit) {
		while (SDL_PollEvent(&e)) {
			switch (e.type) {
				case SDL_KEYDOWN:
					if (e.key.keysym.sym == SDLK_q) {
						quit = true;
					} else if (e.key.keysym.sym == SDLK_SPACE) {
					} else if (e.key.keysym.sym == SDLK_UP) {
					} else if (e.key.keysym.sym == SDLK_RIGHT) {
					} else if (e.key.keysym.sym == SDLK_LEFT) {
					} else if (e.key.keysym.sym == SDLK_DOWN) {
					}
					break;
				default:
					break;
			}
		}
		Tetris::draw();
	}
}

void Tetris::draw()
{
	SDL_Surface *s = SDL_GetWindowSurface(Tetris::window);
	SDL_Rect r{0, 0, 20, 20};
	SDL_FillRect(s, &r, SDL_MapRGB(s->format, 255, 0, 0));
	SDL_UpdateWindowSurface(Tetris::window);
}

Tetris::~Tetris()
{
	SDL_DestroyWindow(Tetris::window);
	SDL_Quit();
}
