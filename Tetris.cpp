#include <iostream>
#include <vector>
#include <SDL2/SDL.h>
#include <stdexcept>
#include "Tetris.h"

Tetris::Tetris(int width, int height)
{
	Tetris::playfield = std::vector<std::vector<int>>(10, std::vector<int>(40, 0));
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
					}
					break;
				default:
					break;
			}
		}
	}
}
