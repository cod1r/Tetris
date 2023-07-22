#include <cstdint>
#include "Tetromino.h"
Tetromino::Tetromino(TetrominoType type, int32_t x1, int32_t y1, int32_t x2, int32_t y2, int32_t x3, int32_t y3,
		int32_t x4, int32_t y4, uint8_t red, uint8_t green, uint8_t blue)
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
Tetromino::Tetromino(const Tetromino& t)
{
	this->type = t.type;
	this->x1 = t.x1;
	this->x2 = t.x2;
	this->x3 = t.x3;
	this->x4 = t.x4;
	this->y1 = t.y1;
	this->y2 = t.y2;
	this->y3 = t.y3;
	this->y4 = t.y4;
	this->red = t.red;
	this->green = t.green;
	this->blue = t.blue;
}
Tetromino::Tetromino()
{}
