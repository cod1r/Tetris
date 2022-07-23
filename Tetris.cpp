#include <iostream>
#include <vector>
#include <SDL2/SDL.h>
#include <stdexcept>
#include <ctime>
#include "Tetris.h"

Tetris::Tetris(int width, int height)
{
	Tetris::playfield = \
		std::vector<std::vector<int>>(Tetris::TETRIS_PLAYFIELD_WIDTH,
				std::vector<int>(Tetris::TETRIS_PLAYFIELD_HEIGHT, 0));
	Tetris::Tetrominos[Tetris::I] = \
		Tetris::Tetromino {
			std::tuple<int, int, int>{
				173,
				216,
				230
			},
			std::array<std::pair<int, int>, 4>{
				std::pair<int, int>{3, 1},
				std::pair<int, int>{4, 1},
				std::pair<int, int>{5, 1},
				std::pair<int, int>{6, 1},
			}
		};
	Tetris::Tetrominos[Tetris::L] = \
		Tetris::Tetromino {
			std::tuple<int, int, int>{
				0,
				0,
				139
			},
			std::array<std::pair<int, int>, 4>{
				std::pair<int, int>{2, 0},
				std::pair<int, int>{3, 0},
				std::pair<int, int>{4, 0},
				std::pair<int, int>{4, 1},
			}
		};
	Tetris::Tetrominos[Tetris::J] = \
		Tetris::Tetromino {
			std::tuple<int, int, int>{
				240,
				165,
				0
			},
			std::array<std::pair<int, int>, 4>{
				std::pair<int, int>{2, 0},
				std::pair<int, int>{2, 1},
				std::pair<int, int>{3, 1},
				std::pair<int, int>{4, 1},
			}
		};
	Tetris::Tetrominos[Tetris::O] = \
		Tetris::Tetromino {
			std::tuple<int, int, int>{
				255,
				255,
				0
			},
			std::array<std::pair<int, int>, 4>{
				std::pair<int, int>{4, 0},
				std::pair<int, int>{5, 0},
				std::pair<int, int>{4, 1},
				std::pair<int, int>{5, 1},
			}
		};
	Tetris::Tetrominos[Tetris::T] = \
		Tetris::Tetromino {
			std::tuple<int, int, int>{
				128,
				0,
				128,
			},
			std::array<std::pair<int, int>, 4>{
				std::pair<int, int>{2, 1},
				std::pair<int, int>{3, 0},
				std::pair<int, int>{3, 1},
				std::pair<int, int>{4, 1},
			}
		};
	Tetris::Tetrominos[Tetris::S] = \
		Tetris::Tetromino {
			std::tuple<int, int, int>{
				0,
				255,
				0
			},
			std::array<std::pair<int, int>, 4>{
				std::pair<int, int>{2, 1},
				std::pair<int, int>{3, 0},
				std::pair<int, int>{3, 1},
				std::pair<int, int>{4, 0},
			}
		};
	Tetris::Tetrominos[Tetris::Z] = \
		Tetris::Tetromino {
			std::tuple<int, int, int>{
				255,
				0,
				0
			},
			std::array<std::pair<int, int>, 4>{
				std::pair<int, int>{2, 0},
				std::pair<int, int>{3, 0},
				std::pair<int, int>{3, 1},
				std::pair<int, int>{4, 1},
			}
		};
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
	Tetris::currentTetromino = Tetris::Tetrominos[Tetris::I];
	Tetris::xleftborder = \
		(width - Tetris::blocksize * Tetris::TETRIS_PLAYFIELD_WIDTH) / 2;
	Tetris::xrightborder = \
		Tetris::xleftborder + \
		(Tetris::blocksize * Tetris::TETRIS_PLAYFIELD_WIDTH);
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
						Tetris::up = true;
					} else if (e.key.keysym.sym == SDLK_RIGHT) {
						Tetris::right = true;
					} else if (e.key.keysym.sym == SDLK_LEFT) {
						Tetris::left = true;
					} else if (e.key.keysym.sym == SDLK_DOWN) {
						Tetris::down = true;
					} else if (e.key.keysym.sym == SDLK_SPACE) {
						Tetris::space = true;
					}
					break;
				case SDL_KEYUP:
					if (e.key.keysym.sym == SDLK_RIGHT) {
						Tetris::right = false;
					} else if (e.key.keysym.sym == SDLK_LEFT) {
						Tetris::left = false;
					} else if (e.key.keysym.sym == SDLK_DOWN) {
						Tetris::down = false;
					} else if (e.key.keysym.sym == SDLK_UP) {
						Tetris::up = false;
					} else if (e.key.keysym.sym == SDLK_SPACE) {
						Tetris::space = false;
					}
					break;
				default:
					break;
			}
		}
		if (Tetris::left) {

		}
		if (Tetris::right) {

		}
		if (Tetris::down) {
			for (int i = 0; i < Tetris::TETROMINO_SIZE; ++i) {
				if (Tetris::currentTetromino.second.at(i).second < Tetris::TETRIS_PLAYFIELD_HEIGHT)
					++Tetris::currentTetromino.second.at(i).second;
			}
		}
		if (Tetris::space) {

		}
		Tetris::draw();
	}
}

void Tetris::draw()
{
	SDL_Surface *s = SDL_GetWindowSurface(Tetris::window);
	SDL_FillRect(s, NULL, SDL_MapRGB(s->format, 0, 0, 0));
	SDL_Rect playfield_area{};
	SDL_FillRect(s, &playfield_area, SDL_MapRGB(s->format, 20, 20, 20));
	for (int i = 0; i < Tetris::TETROMINO_SIZE; ++i) {
		SDL_Rect temp{
			Tetris::xleftborder + Tetris::blocksize * Tetris::currentTetromino.second.at(i).first,
			Tetris::blocksize * Tetris::currentTetromino.second.at(i).second,
			Tetris::blocksize,
			Tetris::blocksize
		};
		SDL_FillRect(
			s,
			&temp,
			SDL_MapRGB(
				s->format,
				std::get<0>(Tetris::currentTetromino.first),
				std::get<1>(Tetris::currentTetromino.first),
				std::get<2>(Tetris::currentTetromino.first)
			)
		);
	}
	SDL_UpdateWindowSurface(Tetris::window);
}

Tetris::~Tetris()
{
	SDL_DestroyWindow(Tetris::window);
	SDL_Quit();
}
