#include <iostream>
#include <vector>
#include <SDL2/SDL.h>
#include <stdexcept>
#include <chrono>
#include <functional>
#include <climits>
#include <random>
#include <set>
#include "Tetris.h"

Tetris::Tetris(int width, int height)
{
	Tetris::playfield = \
		std::vector<std::vector<int>>(Tetris::TETRIS_PLAYFIELD_WIDTH,
				std::vector<int>(Tetris::TETRIS_PLAYFIELD_HEIGHT, 0));
	Tetris::Tetrominos[Tetris::I] = Tetris::Tetromino {
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
	Tetris::Tetrominos[Tetris::L] = Tetris::Tetromino {
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
	Tetris::Tetrominos[Tetris::J] = Tetris::Tetromino {
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
	Tetris::Tetrominos[Tetris::O] = Tetris::Tetromino {
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
	Tetris::Tetrominos[Tetris::T] = Tetris::Tetromino {
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
	Tetris::Tetrominos[Tetris::S] = Tetris::Tetromino {
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
	Tetris::Tetrominos[Tetris::Z] = Tetris::Tetromino {
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
	Tetris::LevelSpeed = std::array<double, 15>{
		1.0,
		0.79300,
		0.6178,
		0.47273,
		0.35520,
		0.26200,
		0.18968,
		0.13473,
		0.09388,
		0.06415,
		0.04298,
		0.02822,
		0.01815,
		0.01144,
		0.00706
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
	Tetris::renderer = SDL_CreateRenderer(window, -1, 0);
	Tetris::generateSequence();
	Tetris::currentTetromino = Tetris::Tetrominos[Tetris::sequence.at(0)];
	Tetris::xleftborder = \
		(width - Tetris::blocksize * Tetris::TETRIS_PLAYFIELD_WIDTH) / 2;
	Tetris::xrightborder = \
		Tetris::xleftborder + \
		(Tetris::blocksize * Tetris::TETRIS_PLAYFIELD_WIDTH);
}

void Tetris::generateSequence()
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> distrib(0, 6);
	std::set<int> seen;
	Tetris::sequence_index = 0;
	while (seen.size() < 7) {
		int a = distrib(gen);
		if (!seen.contains(a)) {
			seen.insert(a);
			Tetris::sequence.at(Tetris::sequence_index++) = a;
		}
	}
}

int Tetris::farLR(bool SIDE)
{
	int far = (SIDE ? INT_MIN:INT_MAX);
	if (!SIDE) {
		for (int i = 0; i < Tetris::TETROMINO_SIZE; ++i) {
			far = SDL_min(far, Tetris::currentTetromino.second.at(i).first);
		}
	} else if (SIDE) {
		for (int i = 0; i < Tetris::TETROMINO_SIZE; ++i) {
			far = SDL_max(far, Tetris::currentTetromino.second.at(i).first);
		}
	}
	return far;
}

int Tetris::farUD(bool SIDE)
{
	int far = (SIDE ? INT_MIN:INT_MAX);
	if (!SIDE) {
		for (int i = 0; i < Tetris::TETROMINO_SIZE; ++i) {
			far = SDL_min(far, Tetris::currentTetromino.second.at(i).second);
		}
	} else if (SIDE) {
		for (int i = 0; i < Tetris::TETROMINO_SIZE; ++i) {
			far = SDL_max(far, Tetris::currentTetromino.second.at(i).second);
		}
	}
	return far;
}

void Tetris::loop()
{
	SDL_Event e;
	std::chrono::time_point<std::chrono::system_clock> prev = std::chrono::system_clock::now();
	std::chrono::time_point<std::chrono::system_clock> lastInputTime = std::chrono::system_clock::now();
	bool quit = false;
	while (!quit || Tetris::level > Tetris::MAX_LEVELS) {
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
		if (Tetris::up) {
			// TODO: rotate the currentTetromino. consider wall kicks as well
		}
		if (
				Tetris::left &&
				std::chrono::duration<double>(std::chrono::system_clock::now() - lastInputTime).count() >= Tetris::SOFT_MOVE_SPEED &&
				Tetris::farLR(false) > 0
		) {
			lastInputTime = std::chrono::system_clock::now();
			for (int i = 0; i < Tetris::TETROMINO_SIZE; ++i) {
				--Tetris::currentTetromino.second.at(i).first;
			}
		}
		if (
				Tetris::right &&
				std::chrono::duration<double>(std::chrono::system_clock::now() - lastInputTime).count() >= Tetris::SOFT_MOVE_SPEED &&
				Tetris::farLR(true) < Tetris::TETRIS_PLAYFIELD_WIDTH - 1
		) {
			lastInputTime = std::chrono::system_clock::now();
			for (int i = 0; i < Tetris::TETROMINO_SIZE; ++i) {
				++Tetris::currentTetromino.second.at(i).first;
			}
		}
		if (
				Tetris::down &&
				std::chrono::duration<double>(std::chrono::system_clock::now() - lastInputTime).count() >= Tetris::SOFT_MOVE_SPEED &&
				Tetris::farUD(true) < Tetris::TETRIS_PLAYFIELD_HEIGHT - 1
		) {
			lastInputTime = std::chrono::system_clock::now();
			for (int i = 0; i < Tetris::TETROMINO_SIZE; ++i) {
				++Tetris::currentTetromino.second.at(i).second;
			}
		}
		if (Tetris::space) {

		}
		std::chrono::time_point<std::chrono::system_clock> curr = std::chrono::system_clock::now();
		bool right_time = \
			std::chrono::duration<double>(curr - prev).count() >= Tetris::LevelSpeed.at(Tetris::level - 1);
		if (right_time && !Tetris::down) {
			prev = std::chrono::system_clock::now();
			for (int i = 0; i < Tetris::TETROMINO_SIZE; ++i) {
				if (Tetris::currentTetromino.second.at(i).second < Tetris::TETRIS_PLAYFIELD_HEIGHT - 1)
					++Tetris::currentTetromino.second.at(i).second;
			}
		}
		Tetris::draw();
	}
}

void Tetris::draw()
{
	SDL_SetRenderDrawColor(Tetris::renderer, 0, 0, 0, 255);
	SDL_RenderClear(Tetris::renderer);
	SDL_SetRenderDrawColor(Tetris::renderer, 100, 100, 100, 255);
	SDL_Rect playfield_area{
		Tetris::xleftborder,
		0,
		Tetris::blocksize * Tetris::TETRIS_PLAYFIELD_WIDTH,
		Tetris::blocksize * Tetris::TETRIS_PLAYFIELD_HEIGHT
	};
	SDL_RenderFillRect(Tetris::renderer, &playfield_area);
	for (int i = 0; i < Tetris::TETROMINO_SIZE; ++i) {
		SDL_SetRenderDrawColor(
			Tetris::renderer,
			std::get<0>(Tetris::currentTetromino.first),
			std::get<1>(Tetris::currentTetromino.first),
			std::get<2>(Tetris::currentTetromino.first),
			255
		);
		SDL_Rect temp {
			Tetris::xleftborder + Tetris::blocksize * Tetris::currentTetromino.second.at(i).first,
			Tetris::blocksize * Tetris::currentTetromino.second.at(i).second,
			Tetris::blocksize,
			Tetris::blocksize
		};
		SDL_RenderFillRect(Tetris::renderer, &temp);
	}
	SDL_RenderPresent(Tetris::renderer);
}

Tetris::~Tetris()
{
	SDL_DestroyRenderer(Tetris::renderer);
	SDL_DestroyWindow(Tetris::window);
	SDL_Quit();
}
