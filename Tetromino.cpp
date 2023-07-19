#include <cstdint>
#include "Tetromino.h"
Tetromino::Tetromino(TetrominoType type, int32_t x1, int32_t y1, int32_t x2, int32_t y2, int32_t x3, int32_t y3,
		int32_t x4, int32_t y4, int8_t red, int8_t green, int8_t blue)
{
	Tetromino::type = type;
	Tetromino::x1 = x1;
	Tetromino::y1 = y1;
	Tetromino::x2 = x2;
	Tetromino::y2 = y2;
	Tetromino::x3 = x3;
	Tetromino::y3 = y3;
	Tetromino::x4 = x4;
	Tetromino::y4 = y4;

	Tetromino::red = red;
	Tetromino::green = green;
	Tetromino::blue = blue;
}
