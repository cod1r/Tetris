#include <cstdint>
#ifndef _TETROMINO_H
#define _TETROMINO_H
enum TetrominoType {
	I,
	J,
	L,
	O,
	S,
	T,
	Z
};
struct Tetromino {
	TetrominoType type;
	int32_t x1;
	int32_t y1;
	int32_t x2;
	int32_t y2;
	int32_t x3;
	int32_t y3;
	int32_t x4;
	int32_t y4;

	uint8_t red;
	uint8_t blue;
	uint8_t green;
	Tetromino();
	Tetromino(const Tetromino&);
	Tetromino(
			TetrominoType,
			int32_t,
			int32_t,
			int32_t,
			int32_t,
			int32_t,
			int32_t,
			int32_t,
			int32_t,
			uint8_t,
			uint8_t,
			uint8_t
	);
};
#endif
