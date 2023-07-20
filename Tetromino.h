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

	int8_t red;
	int8_t blue;
	int8_t green;
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
			int8_t,
			int8_t,
			int8_t
	);
};
#endif
