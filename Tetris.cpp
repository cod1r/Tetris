#include <experimental/simd>
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
#include "Renderer.h"

Tetris::Tetris()
{
	renderer = new Renderer();
	Tetris::Tetrominos = {{
			new Tetromino(TetrominoType::I, 3, 1, 4, 1, 5, 1, 6, 1, 173, 216, 230),
			new Tetromino(TetrominoType::L, 2, 1, 3, 1, 4, 1, 4, 0, 0, 0, 139),
			new Tetromino(TetrominoType::J, 2, 0, 2, 1, 3, 1, 4, 1, 240, 165, 0),
			new Tetromino(TetrominoType::O, 4, 0, 5, 0, 4, 1, 5, 1, 255, 255, 0),
			new Tetromino(TetrominoType::T, 2, 1, 3, 0, 3, 1, 4, 1, 128, 0, 128),
			new Tetromino(TetrominoType::S, 2, 1, 3, 0, 3, 1, 4, 0, 0, 255, 0),
			new Tetromino(TetrominoType::Z, 2, 0, 3, 0, 3, 1, 4, 1, 255, 0, 0),
	}};
	Tetris::LevelSpeed = {{
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
	}};
	Tetris::generate_sequence();
	Tetris::current_tetromino = new Tetromino(*Tetris::Tetrominos[Tetris::sequence.at(Tetris::sequence_index)]);
	renderer->render_tetromino(*Tetris::current_tetromino);
}

bool Tetris::check_fill()
{
	if (Tetris::playfield[current_tetromino->y1][current_tetromino->x1] != -1
			|| Tetris::playfield[current_tetromino->y2][current_tetromino->x2] != -1
			|| Tetris::playfield[current_tetromino->y3][current_tetromino->x3] != -1
			|| Tetris::playfield[current_tetromino->y4][current_tetromino->x4] != -1) {
		return false;
	}
	return true;
}

void Tetris::rotate()
{
}

void Tetris::hard_drop()
{
	int min_add = INT_MAX;

	min_add = SDL_min(min_add,
			TETRIS_PLAYFIELD_HEIGHT - 1 - Tetris::current_tetromino->y1);
	for (int Y = TETRIS_PLAYFIELD_HEIGHT - 1; Y > Tetris::current_tetromino->y1; --Y) {
		if (Tetris::playfield[Y][Tetris::current_tetromino->x1] != -1) {
			min_add = SDL_min(min_add, Y - 1 - Tetris::current_tetromino->y1);
		}
	}

	min_add = SDL_min(min_add,
			TETRIS_PLAYFIELD_HEIGHT - 1 - Tetris::current_tetromino->y2);
	for (int Y = TETRIS_PLAYFIELD_HEIGHT - 1; Y > Tetris::current_tetromino->y2; --Y) {
		if (Tetris::playfield[Y][Tetris::current_tetromino->x2] != -1) {
			min_add = SDL_min(min_add, Y - 1 - Tetris::current_tetromino->y2);
		}
	}

	min_add = SDL_min(min_add,
			TETRIS_PLAYFIELD_HEIGHT - 1 - Tetris::current_tetromino->y3);
	for (int Y = TETRIS_PLAYFIELD_HEIGHT - 1; Y > Tetris::current_tetromino->y3; --Y) {
		if (Tetris::playfield[Y][Tetris::current_tetromino->x3] != -1) {
			min_add = SDL_min(min_add, Y - 1 - Tetris::current_tetromino->y3);
		}
	}

	min_add = SDL_min(min_add,
			TETRIS_PLAYFIELD_HEIGHT - 1 - Tetris::current_tetromino->y4);
	for (int Y = TETRIS_PLAYFIELD_HEIGHT - 1; Y > Tetris::current_tetromino->y4; --Y) {
		if (Tetris::playfield[Y][Tetris::current_tetromino->x4] != -1) {
			min_add = SDL_min(min_add, Y - 1 - Tetris::current_tetromino->y4);
		}
	}

	Tetris::current_tetromino->y1 += min_add;
	Tetris::current_tetromino->y2 += min_add;
	Tetris::current_tetromino->y3 += min_add;
	Tetris::current_tetromino->y4 += min_add;
}

void Tetris::lock()
{
		Tetris::playfield[Tetris::current_tetromino->y1][Tetris::current_tetromino->x1] = Tetris::sequence.at(Tetris::sequence_index);
		Tetris::playfield[Tetris::current_tetromino->y2][Tetris::current_tetromino->x2] = Tetris::sequence.at(Tetris::sequence_index);
		Tetris::playfield[Tetris::current_tetromino->y3][Tetris::current_tetromino->x3] = Tetris::sequence.at(Tetris::sequence_index);
		Tetris::playfield[Tetris::current_tetromino->y4][Tetris::current_tetromino->x4] = Tetris::sequence.at(Tetris::sequence_index);

	++Tetris::sequence_index;
	if (Tetris::sequence_index == 7) {
		Tetris::generate_sequence();
		Tetris::sequence_index = 0;
	}
	Tetris::current_tetromino = new Tetromino(*Tetris::Tetrominos.at(Tetris::sequence.at(Tetris::sequence_index)));
	renderer->render_tetromino(*Tetris::current_tetromino);
}

bool Tetris::check_collision()
{
	if (Tetris::current_tetromino->y1 + 1 == TETRIS_PLAYFIELD_HEIGHT ||
			(Tetris::current_tetromino->y1 + 1 < TETRIS_PLAYFIELD_HEIGHT &&
			 Tetris::playfield[Tetris::current_tetromino->y1 + 1][Tetris::current_tetromino->x1] != -1)
		 ) {
		return true;
	}
	if (Tetris::current_tetromino->y2 + 1 == TETRIS_PLAYFIELD_HEIGHT ||
			(Tetris::current_tetromino->y2 + 1 < TETRIS_PLAYFIELD_HEIGHT &&
			 Tetris::playfield[Tetris::current_tetromino->y2 + 1][Tetris::current_tetromino->x2] != -1)
		 ) {
		return true;
	}
	if (Tetris::current_tetromino->y3 + 1 == TETRIS_PLAYFIELD_HEIGHT ||
			(Tetris::current_tetromino->y3 + 1 < TETRIS_PLAYFIELD_HEIGHT &&
			 Tetris::playfield[Tetris::current_tetromino->y3 + 1][Tetris::current_tetromino->x3] != -1)
		 ) {
		return true;
	}
	if (Tetris::current_tetromino->y4 + 1 == TETRIS_PLAYFIELD_HEIGHT ||
			(Tetris::current_tetromino->y4 + 1 < TETRIS_PLAYFIELD_HEIGHT &&
			 Tetris::playfield[Tetris::current_tetromino->y4 + 1][Tetris::current_tetromino->x4] != -1)
		 ) {
		return true;
	}
	return false;
}

void Tetris::check_line_clear()
{
	std::experimental::fixed_size_simd<int32_t, 10> a(Tetris::playfield[0].data(), std::experimental::element_aligned);
	if (std::experimental::all_of(a != -1)) {
		++Tetris::lines_cleared;
		a = -1;
		a.copy_to(Tetris::playfield[0].data(), std::experimental::element_aligned);
	}
	std::experimental::fixed_size_simd<int32_t, 10> a1(Tetris::playfield[1].data(), std::experimental::element_aligned);
	if (std::experimental::all_of(a1 != -1)) {
		++Tetris::lines_cleared;
		a1 = -1;
		a1.copy_to(Tetris::playfield[1].data(), std::experimental::element_aligned);
		for (int Y2 = 1; Y2 > 0; --Y2) {
			for (int X2 = 0; X2 < TETRIS_PLAYFIELD_WIDTH; ++X2) {
				std::swap(Tetris::playfield[Y2][X2], Tetris::playfield[Y2 - 1][X2]);
			}
		}
	}
	std::experimental::fixed_size_simd<int32_t, 10> a2(Tetris::playfield[2].data(), std::experimental::element_aligned);
	if (std::experimental::all_of(a2 != -1)) {
		++Tetris::lines_cleared;
		a2 = -1;
		a2.copy_to(Tetris::playfield[2].data(), std::experimental::element_aligned);
		for (int Y2 = 2; Y2 > 0; --Y2) {
			for (int X2 = 0; X2 < TETRIS_PLAYFIELD_WIDTH; ++X2) {
				std::swap(Tetris::playfield[Y2][X2], Tetris::playfield[Y2 - 1][X2]);
			}
		}
	}
	std::experimental::fixed_size_simd<int32_t, 10> a3(Tetris::playfield[3].data(), std::experimental::element_aligned);
	if (std::experimental::all_of(a3 != -1)) {
		++Tetris::lines_cleared;
		a3 = -1;
		a3.copy_to(Tetris::playfield[3].data(), std::experimental::element_aligned);
		for (int Y2 = 3; Y2 > 0; --Y2) {
			for (int X2 = 0; X2 < TETRIS_PLAYFIELD_WIDTH; ++X2) {
				std::swap(Tetris::playfield[Y2][X2], Tetris::playfield[Y2 - 1][X2]);
			}
		}
	}
	std::experimental::fixed_size_simd<int32_t, 10> a4(Tetris::playfield[4].data(), std::experimental::element_aligned);
	if (std::experimental::all_of(a4 != -1)) {
		++Tetris::lines_cleared;
		a4 = -1;
		a4.copy_to(Tetris::playfield[4].data(), std::experimental::element_aligned);
		for (int Y2 = 4; Y2 > 0; --Y2) {
			for (int X2 = 0; X2 < TETRIS_PLAYFIELD_WIDTH; ++X2) {
				std::swap(Tetris::playfield[Y2][X2], Tetris::playfield[Y2 - 1][X2]);
			}
		}
	}
	std::experimental::fixed_size_simd<int32_t, 10> a5(Tetris::playfield[5].data(), std::experimental::element_aligned);
	if (std::experimental::all_of(a5 != -1)) {
		++Tetris::lines_cleared;
		a5 = -1;
		a5.copy_to(Tetris::playfield[5].data(), std::experimental::element_aligned);
		for (int Y2 = 5; Y2 > 0; --Y2) {
			for (int X2 = 0; X2 < TETRIS_PLAYFIELD_WIDTH; ++X2) {
				std::swap(Tetris::playfield[Y2][X2], Tetris::playfield[Y2 - 1][X2]);
			}
		}
	}
	std::experimental::fixed_size_simd<int32_t, 10> a6(Tetris::playfield[6].data(), std::experimental::element_aligned);
	if (std::experimental::all_of(a6 != -1)) {
		++Tetris::lines_cleared;
		a6 = -1;
		a6.copy_to(Tetris::playfield[6].data(), std::experimental::element_aligned);
		for (int Y2 = 6; Y2 > 0; --Y2) {
			for (int X2 = 0; X2 < TETRIS_PLAYFIELD_WIDTH; ++X2) {
				std::swap(Tetris::playfield[Y2][X2], Tetris::playfield[Y2 - 1][X2]);
			}
		}
	}
	std::experimental::fixed_size_simd<int32_t, 10> a7(Tetris::playfield[7].data(), std::experimental::element_aligned);
	if (std::experimental::all_of(a7 != -1)) {
		++Tetris::lines_cleared;
		a7 = -1;
		a7.copy_to(Tetris::playfield[7].data(), std::experimental::element_aligned);
		for (int Y2 = 7; Y2 > 0; --Y2) {
			for (int X2 = 0; X2 < TETRIS_PLAYFIELD_WIDTH; ++X2) {
				std::swap(Tetris::playfield[Y2][X2], Tetris::playfield[Y2 - 1][X2]);
			}
		}
	}
	std::experimental::fixed_size_simd<int32_t, 10> a8(Tetris::playfield[8].data(), std::experimental::element_aligned);
	if (std::experimental::all_of(a8 != -1)) {
		++Tetris::lines_cleared;
		a8 = -1;
		a8.copy_to(Tetris::playfield[8].data(), std::experimental::element_aligned);
		for (int Y2 = 8; Y2 > 0; --Y2) {
			for (int X2 = 0; X2 < TETRIS_PLAYFIELD_WIDTH; ++X2) {
				std::swap(Tetris::playfield[Y2][X2], Tetris::playfield[Y2 - 1][X2]);
			}
		}
	}
	std::experimental::fixed_size_simd<int32_t, 10> a9(Tetris::playfield[9].data(), std::experimental::element_aligned);
	if (std::experimental::all_of(a9 != -1)) {
		++Tetris::lines_cleared;
		a9 = -1;
		a9.copy_to(Tetris::playfield[9].data(), std::experimental::element_aligned);
		for (int Y2 = 9; Y2 > 0; --Y2) {
			for (int X2 = 0; X2 < TETRIS_PLAYFIELD_WIDTH; ++X2) {
				std::swap(Tetris::playfield[Y2][X2], Tetris::playfield[Y2 - 1][X2]);
			}
		}
	}
	std::experimental::fixed_size_simd<int32_t, 10> a10(Tetris::playfield[10].data(), std::experimental::element_aligned);
	if (std::experimental::all_of(a10 != -1)) {
		++Tetris::lines_cleared;
		a10 = -1;
		a10.copy_to(Tetris::playfield[10].data(), std::experimental::element_aligned);
		for (int Y2 = 10; Y2 > 0; --Y2) {
			for (int X2 = 0; X2 < TETRIS_PLAYFIELD_WIDTH; ++X2) {
				std::swap(Tetris::playfield[Y2][X2], Tetris::playfield[Y2 - 1][X2]);
			}
		}
	}
	std::experimental::fixed_size_simd<int32_t, 10> a11(Tetris::playfield[11].data(), std::experimental::element_aligned);
	if (std::experimental::all_of(a11 != -1)) {
		++Tetris::lines_cleared;
		a11 = -1;
		a11.copy_to(Tetris::playfield[11].data(), std::experimental::element_aligned);
		for (int Y2 = 11; Y2 > 0; --Y2) {
			for (int X2 = 0; X2 < TETRIS_PLAYFIELD_WIDTH; ++X2) {
				std::swap(Tetris::playfield[Y2][X2], Tetris::playfield[Y2 - 1][X2]);
			}
		}
	}
	std::experimental::fixed_size_simd<int32_t, 10> a12(Tetris::playfield[12].data(), std::experimental::element_aligned);
	if (std::experimental::all_of(a12 != -1)) {
		++Tetris::lines_cleared;
		a12 = -1;
		a12.copy_to(Tetris::playfield[12].data(), std::experimental::element_aligned);
		for (int Y2 = 12; Y2 > 0; --Y2) {
			for (int X2 = 0; X2 < TETRIS_PLAYFIELD_WIDTH; ++X2) {
				std::swap(Tetris::playfield[Y2][X2], Tetris::playfield[Y2 - 1][X2]);
			}
		}
	}
	std::experimental::fixed_size_simd<int32_t, 10> a13(Tetris::playfield[13].data(), std::experimental::element_aligned);
	if (std::experimental::all_of(a13 != -1)) {
		++Tetris::lines_cleared;
		a13 = -1;
		a13.copy_to(Tetris::playfield[13].data(), std::experimental::element_aligned);
		for (int Y2 = 13; Y2 > 0; --Y2) {
			for (int X2 = 0; X2 < TETRIS_PLAYFIELD_WIDTH; ++X2) {
				std::swap(Tetris::playfield[Y2][X2], Tetris::playfield[Y2 - 1][X2]);
			}
		}
	}
	std::experimental::fixed_size_simd<int32_t, 10> a14(Tetris::playfield[14].data(), std::experimental::element_aligned);
	if (std::experimental::all_of(a14 != -1)) {
		++Tetris::lines_cleared;
		a14 = -1;
		a14.copy_to(Tetris::playfield[14].data(), std::experimental::element_aligned);
		for (int Y2 = 14; Y2 > 0; --Y2) {
			for (int X2 = 0; X2 < TETRIS_PLAYFIELD_WIDTH; ++X2) {
				std::swap(Tetris::playfield[Y2][X2], Tetris::playfield[Y2 - 1][X2]);
			}
		}
	}
	std::experimental::fixed_size_simd<int32_t, 10> a15(Tetris::playfield[15].data(), std::experimental::element_aligned);
	if (std::experimental::all_of(a15 != -1)) {
		++Tetris::lines_cleared;
		a15 = -1;
		a15.copy_to(Tetris::playfield[15].data(), std::experimental::element_aligned);
		for (int Y2 = 15; Y2 > 0; --Y2) {
			for (int X2 = 0; X2 < TETRIS_PLAYFIELD_WIDTH; ++X2) {
				std::swap(Tetris::playfield[Y2][X2], Tetris::playfield[Y2 - 1][X2]);
			}
		}
	}
	std::experimental::fixed_size_simd<int32_t, 10> a16(Tetris::playfield[16].data(), std::experimental::element_aligned);
	if (std::experimental::all_of(a16 != -1)) {
		++Tetris::lines_cleared;
		a16 = -1;
		a16.copy_to(Tetris::playfield[16].data(), std::experimental::element_aligned);
		for (int Y2 = 16; Y2 > 0; --Y2) {
			for (int X2 = 0; X2 < TETRIS_PLAYFIELD_WIDTH; ++X2) {
				std::swap(Tetris::playfield[Y2][X2], Tetris::playfield[Y2 - 1][X2]);
			}
		}
	}
	std::experimental::fixed_size_simd<int32_t, 10> a17(Tetris::playfield[17].data(), std::experimental::element_aligned);
	if (std::experimental::all_of(a17 != -1)) {
		++Tetris::lines_cleared;
		a17 = -1;
		a17.copy_to(Tetris::playfield[17].data(), std::experimental::element_aligned);
		for (int Y2 = 17; Y2 > 0; --Y2) {
			for (int X2 = 0; X2 < TETRIS_PLAYFIELD_WIDTH; ++X2) {
				std::swap(Tetris::playfield[Y2][X2], Tetris::playfield[Y2 - 1][X2]);
			}
		}
	}
	std::experimental::fixed_size_simd<int32_t, 10> a18(Tetris::playfield[18].data(), std::experimental::element_aligned);
	if (std::experimental::all_of(a18 != -1)) {
		++Tetris::lines_cleared;
		a18 = -1;
		a18.copy_to(Tetris::playfield[18].data(), std::experimental::element_aligned);
		for (int Y2 = 18; Y2 > 0; --Y2) {
			for (int X2 = 0; X2 < TETRIS_PLAYFIELD_WIDTH; ++X2) {
				std::swap(Tetris::playfield[Y2][X2], Tetris::playfield[Y2 - 1][X2]);
			}
		}
	}
	std::experimental::fixed_size_simd<int32_t, 10> a19(Tetris::playfield[19].data(), std::experimental::element_aligned);
	if (std::experimental::all_of(a19 != -1)) {
		++Tetris::lines_cleared;
		a19 = -1;
		a19.copy_to(Tetris::playfield[19].data(), std::experimental::element_aligned);
		for (int Y2 = 19; Y2 > 0; --Y2) {
			for (int X2 = 0; X2 < TETRIS_PLAYFIELD_WIDTH; ++X2) {
				std::swap(Tetris::playfield[Y2][X2], Tetris::playfield[Y2 - 1][X2]);
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
	if (Tetris::current_tetromino->x1 - 1 < 0 ||
			(Tetris::current_tetromino->x1 - 1 >= 0 &&
			 Tetris::playfield[Tetris::current_tetromino->y1][Tetris::current_tetromino->x1 - 1] != -1))
		return false;
	if (Tetris::current_tetromino->x2 - 1 < 0 ||
			(Tetris::current_tetromino->x2 - 1 >= 0 &&
			 Tetris::playfield[Tetris::current_tetromino->y2][Tetris::current_tetromino->x2 - 1] != -1))
		return false;
	if (Tetris::current_tetromino->x3 - 1 < 0 ||
			(Tetris::current_tetromino->x3 - 1 >= 0 &&
			 Tetris::playfield[Tetris::current_tetromino->y3][Tetris::current_tetromino->x3 - 1] != -1))
		return false;
	if (Tetris::current_tetromino->x4 - 1 < 0 ||
			(Tetris::current_tetromino->x4 - 1 >= 0 &&
			 Tetris::playfield[Tetris::current_tetromino->y4][Tetris::current_tetromino->x4 - 1] != -1))
		return false;
	return true;
}

bool Tetris::check_right()
{
	if (Tetris::current_tetromino->x1 + 1 == TETRIS_PLAYFIELD_WIDTH ||
			(Tetris::current_tetromino->x1 + 1 < TETRIS_PLAYFIELD_WIDTH &&
			 Tetris::playfield[Tetris::current_tetromino->y1][Tetris::current_tetromino->x1 + 1] != -1))
		return false;
	if (Tetris::current_tetromino->x2 + 1 == TETRIS_PLAYFIELD_WIDTH ||
			(Tetris::current_tetromino->x2 + 1 < TETRIS_PLAYFIELD_WIDTH &&
			 Tetris::playfield[Tetris::current_tetromino->y2][Tetris::current_tetromino->x2 + 1] != -1))
		return false;
	if (Tetris::current_tetromino->x3 + 1 == TETRIS_PLAYFIELD_WIDTH ||
			(Tetris::current_tetromino->x3 + 1 < TETRIS_PLAYFIELD_WIDTH &&
			 Tetris::playfield[Tetris::current_tetromino->y3][Tetris::current_tetromino->x3 + 1] != -1))
		return false;
	if (Tetris::current_tetromino->x4 + 1 == TETRIS_PLAYFIELD_WIDTH ||
			(Tetris::current_tetromino->x4 + 1 < TETRIS_PLAYFIELD_WIDTH &&
			 Tetris::playfield[Tetris::current_tetromino->y4][Tetris::current_tetromino->x4 + 1] != -1))
		return false;
	return true;
}

bool Tetris::check_down()
{
	if (Tetris::current_tetromino->y1 + 1 == TETRIS_PLAYFIELD_HEIGHT ||
			(Tetris::current_tetromino->y1 + 1 < TETRIS_PLAYFIELD_HEIGHT &&
			 Tetris::playfield[Tetris::current_tetromino->y1 + 1][Tetris::current_tetromino->x1] != -1)) {
		return false;
	}
	if (Tetris::current_tetromino->y2 + 1 == TETRIS_PLAYFIELD_HEIGHT ||
			(Tetris::current_tetromino->y2 + 1 < TETRIS_PLAYFIELD_HEIGHT &&
			 Tetris::playfield[Tetris::current_tetromino->y2 + 1][Tetris::current_tetromino->x2] != -1)) {
		return false;
	}
	if (Tetris::current_tetromino->y3 + 1 == TETRIS_PLAYFIELD_HEIGHT ||
			(Tetris::current_tetromino->y3 + 1 < TETRIS_PLAYFIELD_HEIGHT &&
			 Tetris::playfield[Tetris::current_tetromino->y3 + 1][Tetris::current_tetromino->x3] != -1)) {
		return false;
	}
	if (Tetris::current_tetromino->y4 + 1 == TETRIS_PLAYFIELD_HEIGHT ||
			(Tetris::current_tetromino->y4 + 1 < TETRIS_PLAYFIELD_HEIGHT &&
			 Tetris::playfield[Tetris::current_tetromino->y4 + 1][Tetris::current_tetromino->x4] != -1)) {
		return false;
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
				std::chrono::duration<double>(std::chrono::system_clock::now() -
					last_input_time_up).count() >= Tetris::ROTATE_SPEED
		) {
			last_input_time_up = std::chrono::system_clock::now();
			Tetris::rotate();
		}
		if (
				Tetris::left &&
				std::chrono::duration<double>(std::chrono::system_clock::now() -
					last_input_time_left).count() >= Tetris::SOFT_MOVE_SPEED &&
				Tetris::check_left()
		) {
			last_input_time_left = std::chrono::system_clock::now();
			--Tetris::current_tetromino->x1;
			--Tetris::current_tetromino->x2;
			--Tetris::current_tetromino->x3;
			--Tetris::current_tetromino->x4;
		}
		if (
				Tetris::right &&
				std::chrono::duration<double>(std::chrono::system_clock::now() - \
					last_input_time_right).count() >= Tetris::SOFT_MOVE_SPEED &&
				Tetris::check_right()
		) {
			last_input_time_right = std::chrono::system_clock::now();
			++Tetris::current_tetromino->x1;
			++Tetris::current_tetromino->x2;
			++Tetris::current_tetromino->x3;
			++Tetris::current_tetromino->x4;
		}
		if (
				Tetris::down &&
				std::chrono::duration<double>(std::chrono::system_clock::now() - \
					last_input_time_down).count() >= Tetris::SOFT_MOVE_SPEED &&
				Tetris::check_down()
		) {
			last_input_time_down = std::chrono::system_clock::now();
			++Tetris::current_tetromino->y1;
			++Tetris::current_tetromino->y2;
			++Tetris::current_tetromino->y3;
			++Tetris::current_tetromino->y4;
		}

		if (Tetris::space) {
			Tetris::hard_drop();
		}

		std::chrono::time_point<std::chrono::system_clock> curr = std::chrono::system_clock::now();
		bool right_time =
			std::chrono::duration<double>(curr - prev).count() >= Tetris::LevelSpeed.at(Tetris::level);

		if (right_time && !Tetris::down && Tetris::check_down()) {
			prev = std::chrono::system_clock::now();
			++Tetris::current_tetromino->y1;
			++Tetris::current_tetromino->y2;
			++Tetris::current_tetromino->y3;
			++Tetris::current_tetromino->y4;
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
		renderer->update_tetromino(*Tetris::current_tetromino);
		renderer->render();
	}
}
