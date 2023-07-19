#include <GL/glew.h>
#include <string>
#include <cstdint>
#include <iostream>
#include <fstream>
#include <vector>
#include <SDL2/SDL.h>
#include <stdexcept>
#include <chrono>
#include <climits>
#include <random>
#include <set>
#include "Tetris.h"
#include "Tetromino.h"

Tetris::Tetris()
{
	Tetris::Tetrominos[0] = Tetris::Tetromino(TetrominoType::I, 3, 1, 4, 1, 5, 1, 6, 1, 173, 216, 230);
	Tetris::Tetrominos[1] = Tetris::Tetromino(TetrominoType::L, 2, 1, 3, 1, 4, 1, 4, 0, 0, 0, 139);
	Tetris::Tetrominos[2] = Tetris::Tetromino(TetrominoType::J, 2, 0, 2, 1, 3, 1, 4, 1, 240, 165, 0);
	Tetris::Tetrominos[3] = Tetris::Tetromino(TetrominoType::O, 4, 0, 5, 0, 4, 1, 5, 1, 255, 255, 0);
	Tetris::Tetrominos[4] = Tetris::Tetromino(TetrominoType::T, 2, 1, 3, 0, 3, 1, 4, 1, 128, 0, 128);
	Tetris::Tetrominos[5] = Tetris::Tetromino(TetrominoType::S, 2, 1, 3, 0, 3, 1, 4, 0, 0, 255, 0);
	Tetris::Tetrominos[6] = Tetris::Tetromino(TetrominoType::Z, 2, 0, 3, 0, 3, 1, 4, 1, 255, 0, 0);
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
	Tetris::generate_sequence();
	Tetris::current_tetromino = Tetris::Tetrominos[Tetris::sequence.at(Tetris::sequence_index)];
}

bool Tetris::checkFill()
{
	for (std::pair<int, int> p : Tetris::current_tetromino.second) {
		if (Tetris::playfield[p.second][p.first] != -1) {
			return false;
		}
	}
	return true;
}

void Tetris::rotate()
{
	Tetris::Tetromino tempT = Tetris::current_tetromino;
	if (Tetris::sequence.at(Tetris::sequence_index) == Tetris::I ||
			Tetris::sequence.at(Tetris::sequence_index) == Tetris::O) {
		const int rotateMatrixSize = 4;
		for (int t = 0; t < Tetris::TETROMINO_SIZE; ++t) {
			tempT.second.at(t).first -= std::get<3>(tempT.first).first;
			tempT.second.at(t).second -= std::get<3>(tempT.first).second;

			int old_c = tempT.second.at(t).first;
			tempT.second.at(t).first = rotateMatrixSize - 1 - tempT.second.at(t).second;
			tempT.second.at(t).second = old_c;

			tempT.second.at(t).first += std::get<3>(tempT.first).first;
			tempT.second.at(t).second += std::get<3>(tempT.first).second;
		}
	} else {
		const int rotateMatrixSize = 3;
		for (int t = 0; t < Tetris::TETROMINO_SIZE; ++t) {
			tempT.second.at(t).first -= std::get<3>(tempT.first).first;
			tempT.second.at(t).second -= std::get<3>(tempT.first).second;

			int old_c = tempT.second.at(t).first;
			tempT.second.at(t).first = rotateMatrixSize - 1 - tempT.second.at(t).second;
			tempT.second.at(t).second = old_c;

			tempT.second.at(t).first += std::get<3>(tempT.first).first;
			tempT.second.at(t).second += std::get<3>(tempT.first).second;
		}
	}
	bool swappable = true;
	int miny = INT_MAX;
	int maxy = INT_MIN;
	int minx = INT_MAX;
	int maxx = INT_MIN;
	for (int t = 0; t < Tetris::TETROMINO_SIZE; ++t) {
		miny = SDL_min(miny, tempT.second.at(t).second);
		maxy = SDL_max(maxy, tempT.second.at(t).second);
		minx = SDL_min(minx, tempT.second.at(t).first);
		maxx = SDL_max(maxx, tempT.second.at(t).first);
	}
	if (miny < 0) {
		for (int t = 0; t < Tetris::TETROMINO_SIZE; ++t) {
			tempT.second.at(t).second += SDL_abs(miny);
		}
	} else if (maxy >= Tetris::TETRIS_PLAYFIELD_HEIGHT) {
		for (int t = 0; t < Tetris::TETROMINO_SIZE; ++t) {
			tempT.second.at(t).second -= SDL_abs(maxy - (Tetris::TETRIS_PLAYFIELD_HEIGHT - 1));
		}
	}

	if (minx < 0) {
		for (int t = 0; t < Tetris::TETROMINO_SIZE; ++t) {
			tempT.second.at(t).first += SDL_abs(minx);
		}
	} else if (maxx >= Tetris::TETRIS_PLAYFIELD_WIDTH) {
		for (int t = 0; t < Tetris::TETROMINO_SIZE; ++t) {
			tempT.second.at(t).first -= SDL_abs(maxx - (Tetris::TETRIS_PLAYFIELD_WIDTH - 1));
		}
	}
	for (std::pair<int, int> p : tempT.second) {
		if (p.first < 0 ||
				p.first >= Tetris::TETRIS_PLAYFIELD_WIDTH ||
				p.second < 0 ||
				p.second >= Tetris::TETRIS_PLAYFIELD_HEIGHT ||
				Tetris::playfield[p.second][p.first] != -1
		) {
			swappable = false;
		}
	}
	if (swappable) {
		Tetris::current_tetromino = tempT;
	}
}

void Tetris::hard_drop()
{
	int min_add = INT_MAX;
	for (int t = 0; t < Tetris::TETROMINO_SIZE; ++t) {
		min_add = SDL_min(min_add, Tetris::TETRIS_PLAYFIELD_HEIGHT - 1 - Tetris::current_tetromino.second.at(t).second);
		for (int Y = Tetris::TETRIS_PLAYFIELD_HEIGHT - 1; Y > Tetris::current_tetromino.second.at(t).second; --Y) {
			if (Tetris::playfield[Y][Tetris::current_tetromino.second.at(t).first] != -1) {
				min_add = SDL_min(min_add, Y - 1 - Tetris::current_tetromino.second.at(t).second);
			}
		}
	}
	std::get<3>(Tetris::current_tetromino.first).second += min_add;
	for (int t = 0; t < Tetris::TETROMINO_SIZE; ++t) {
		Tetris::current_tetromino.second.at(t).second += min_add;
	}
}

void Tetris::lock()
{
	for (int i = 0; i < Tetris::TETROMINO_SIZE; ++i) {
		Tetris::playfield[Tetris::current_tetromino.second.at(i).second][Tetris::current_tetromino.second.at(i).first] = \
			Tetris::sequence.at(Tetris::sequence_index);
	}
	++Tetris::sequence_index;
	if (Tetris::sequence_index == 7) {
		Tetris::generate_sequence();
		Tetris::sequence_index = 0;
	}
	Tetris::current_tetromino = Tetris::Tetrominos.at(Tetris::sequence.at(Tetris::sequence_index));
}

bool Tetris::check_collision()
{
	for (int i = 0; i < Tetris::TETROMINO_SIZE; ++i) {
		if (Tetris::current_tetromino.second.at(i).second + 1 == Tetris::TETRIS_PLAYFIELD_HEIGHT ||
				(Tetris::current_tetromino.second.at(i).second + 1 < Tetris::TETRIS_PLAYFIELD_HEIGHT &&
				Tetris::playfield[Tetris::current_tetromino.second.at(i).second + 1][Tetris::current_tetromino.second.at(i).first] != -1)
		) {
				return true;
		}
	}
	return false;
}

void Tetris::check_line_clear()
{
	for (int Y = 0; Y < Tetris::TETRIS_PLAYFIELD_HEIGHT; ++Y) {
		int counter = 0;
		for (int X = 0; X < Tetris::TETRIS_PLAYFIELD_WIDTH; ++X) {
			if (Tetris::playfield[Y][X] != -1) {
				++counter;
			}
		}
		if (counter == Tetris::TETRIS_PLAYFIELD_WIDTH) {
			++Tetris::lines_cleared;
			for (int X2 = 0; X2 < Tetris::TETRIS_PLAYFIELD_WIDTH; ++X2) {
				Tetris::playfield[Y][X2] = -1;
			}
			for (int Y2 = Y; Y2 > 0; --Y2) {
				for (int X2 = 0; X2 < Tetris::TETRIS_PLAYFIELD_WIDTH; ++X2) {
					std::swap(Tetris::playfield[Y2][X2], Tetris::playfield[Y2 - 1][X2]);
				}
			}
		}
	}
	if (Tetris::lines_cleared == Tetris::LINE_CLEAR_LEVELUP_AMOUNT) {
		++Tetris::level;
		Tetris::lines_cleared = 0;
	}
}

void Tetris::generate_sequence()
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> distrib(0, 6);
	std::set<int> seen;
	int temp_sequence_index = 0;
	while (seen.size() < 7) {
		int a = distrib(gen);
		if (!seen.contains(a)) {
			seen.insert(a);
			Tetris::sequence.at(temp_sequence_index++) = a;
		}
	}
}

bool Tetris::check_left()
{
	for (int i = 0; i < Tetris::TETROMINO_SIZE; ++i) {
		if (Tetris::current_tetromino.second.at(i).first - 1 < 0 ||
				(Tetris::current_tetromino.second.at(i).first - 1 >= 0 &&
				 Tetris::playfield[Tetris::current_tetromino.second.at(i).second][Tetris::current_tetromino.second.at(i).first - 1] != -1))
			return false;
	}
	return true;
}

bool Tetris::check_right()
{
	for (int i = 0; i < Tetris::TETROMINO_SIZE; ++i) {
		if (Tetris::current_tetromino.second.at(i).first + 1 == Tetris::TETRIS_PLAYFIELD_WIDTH ||
				(Tetris::current_tetromino.second.at(i).first + 1 < Tetris::TETRIS_PLAYFIELD_WIDTH &&
				 Tetris::playfield[Tetris::current_tetromino.second.at(i).second][Tetris::current_tetromino.second.at(i).first + 1] != -1))
			return false;
	}
	return true;
}

bool Tetris::check_down()
{
	for (int i = 0; i < Tetris::TETROMINO_SIZE; ++i) {
		if (Tetris::current_tetromino.second.at(i).second + 1 == Tetris::TETRIS_PLAYFIELD_HEIGHT ||
				(Tetris::current_tetromino.second.at(i).second + 1 < Tetris::TETRIS_PLAYFIELD_HEIGHT &&
				 Tetris::playfield[Tetris::current_tetromino.second.at(i).second + 1][Tetris::current_tetromino.second.at(i).first] != -1)) {
			return false;
		}
	}
	return true;
}

void Tetris::loop()
{
	SDL_Event e;
	std::chrono::time_point<std::chrono::system_clock> prev = std::chrono::system_clock::now();
	std::chrono::time_point<std::chrono::system_clock> last_input_time_down = std::chrono::system_clock::now();
	std::chrono::time_point<std::chrono::system_clock> last_input_time_left = std::chrono::system_clock::now();
	std::chrono::time_point<std::chrono::system_clock> last_input_time_right = std::chrono::system_clock::now();
	std::chrono::time_point<std::chrono::system_clock> last_input_time_up = std::chrono::system_clock::now();
	std::chrono::time_point<std::chrono::system_clock> collide_time;
	bool collided = false;
	while (Tetris::level < Tetris::MAX_LEVELS && Tetris::check_fill()) {
		while (SDL_PollEvent(&e)) {
			switch (e.type) {
				case SDL_KEYDOWN:
					if (e.key.keysym.sym == SDLK_q) {
						return;
					} else if (e.key.keysym.sym == SDLK_SPACE) {
						Tetris::space = true;
					} else if (e.key.keysym.sym == SDLK_UP) {
						Tetris::up = true;
					} else if (e.key.keysym.sym == SDLK_RIGHT) {
						Tetris::right = true;
					} else if (e.key.keysym.sym == SDLK_LEFT) {
						Tetris::left = true;
					} else if (e.key.keysym.sym == SDLK_DOWN) {
						Tetris::down = true;
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
		if (Tetris::up &&
				std::chrono::duration<double>(std::chrono::system_clock::now() - \
					last_input_time_up).count() >= Tetris::ROTATE_SPEED
		) {
			last_input_time_up = std::chrono::system_clock::now();
			Tetris::rotate();
		}
		if (
				Tetris::left &&
				std::chrono::duration<double>(std::chrono::system_clock::now() - \
					last_input_time_left).count() >= Tetris::SOFT_MOVE_SPEED &&
				Tetris::check_left()
		) {
			last_input_time_left = std::chrono::system_clock::now();
			--std::get<3>(Tetris::current_tetromino.first).first;
			for (int i = 0; i < Tetris::TETROMINO_SIZE; ++i) {
				--Tetris::current_tetromino.second.at(i).first;
			}
		}
		if (
				Tetris::right &&
				std::chrono::duration<double>(std::chrono::system_clock::now() - \
					last_input_time_right).count() >= Tetris::SOFT_MOVE_SPEED &&
				Tetris::check_right()
		) {
			last_input_time_right = std::chrono::system_clock::now();
			++std::get<3>(Tetris::current_tetromino.first).first;
			for (int i = 0; i < Tetris::TETROMINO_SIZE; ++i) {
				++Tetris::current_tetromino.second.at(i).first;
			}
		}
		if (
				Tetris::down &&
				std::chrono::duration<double>(std::chrono::system_clock::now() - \
					last_input_time_down).count() >= Tetris::SOFT_MOVE_SPEED &&
				Tetris::check_down()
		) {
			last_input_time_down = std::chrono::system_clock::now();
			++std::get<3>(Tetris::current_tetromino.first).second;
			for (int i = 0; i < Tetris::TETROMINO_SIZE; ++i) {
				++Tetris::current_tetromino.second.at(i).second;
			}
		}

		if (Tetris::space) {
			Tetris::hard_drop();
		}

		std::chrono::time_point<std::chrono::system_clock> curr = std::chrono::system_clock::now();
		bool right_time = \
			std::chrono::duration<double>(curr - prev).count() >= Tetris::LevelSpeed.at(Tetris::level);

		if (right_time && !Tetris::down && Tetris::check_down()) {
			prev = std::chrono::system_clock::now();
			++std::get<3>(Tetris::current_tetromino.first).second;
			for (int i = 0; i < Tetris::TETROMINO_SIZE; ++i) {
				++Tetris::current_tetromino.second.at(i).second;
			}
		}

		if (Tetris::check_collision()) {
			if (!collided) {
				collide_time = std::chrono::system_clock::now();
			}
			collided = true;
		} else {
			collided = false;
		}
		if (collided &&
				std::chrono::duration<double>(std::chrono::system_clock::now() - collide_time).count() >= Tetris::LOCK_DELAY) {
			collide_time = std::chrono::system_clock::now();
			Tetris::lock();
			collided = false;
		}
		Tetris::check_line_clear();
		Tetris::draw();
	}
}
