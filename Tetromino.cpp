#include "Tetromino.h"
#include <cstdint>
#include <iostream>
Tetromino::Tetromino(TetrominoType type, int32_t x1, int32_t y1, int32_t x2,
                     int32_t y2, int32_t x3, int32_t y3, int32_t x4, int32_t y4,
                     uint8_t red, uint8_t green, uint8_t blue) {
  this->type = type;
  this->x1 = x1;
  this->y1 = y1;
  this->x2 = x2;
  this->y2 = y2;
  this->x3 = x3;
  this->y3 = y3;
  this->x4 = x4;
  this->y4 = y4;

  this->red = red;
  this->green = green;
  this->blue = blue;

  this->state = TetrominoState::BOTTOM;
}
Tetromino::Tetromino(const Tetromino &t) {
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
  this->state = t.state;
}
Tetromino &Tetromino::operator=(const Tetromino &t) {
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
  this->state = t.state;
  return *this;
}
Tetromino::Tetromino() {}

Tetromino Tetromino::rotate() {
  Tetromino temp = *this;
  switch (this->state) {
  case TetrominoState::BOTTOM: {
    switch (this->type) {
    case TetrominoType::I: {
      temp.x1 += 1;
      temp.y1 -= 2;
      temp.y2 -= 1;
      temp.x3 -= 1;
      temp.x4 -= 2;
      temp.y4 += 1;
      break;
    }
    case TetrominoType::J: {
      temp.x1 += 1;
      temp.y2 -= 1;
      temp.x3 -= 1;
      temp.x4 -= 2;
      temp.y4 += 1;
      break;
    }
    case TetrominoType::L: {
      temp.x1 += 1;
      temp.y1 -= 2;
      temp.y2 -= 1;
      temp.x3 -= 1;
      temp.y4 += 1;
      break;
    }
    case TetrominoType::O: {
      break;
    }
    case TetrominoType::S: {
      temp.y1 -= 2;
      temp.x2 -= 1;
      temp.y2 -= 1;
      temp.x4 -= 1;
      temp.y4 += 1;
      break;
    }
    case TetrominoType::Z: {
      temp.x1 += 1;
      temp.y1 -= 1;
      temp.x3 -= 1;
      temp.y3 -= 1;
      temp.x4 -= 2;
      break;
    }
    case TetrominoType::T: {
      temp.x1 += 1;
      temp.y1 -= 1;
      temp.x2 += 1;
      temp.y2 += 1;
      temp.x4 -= 1;
      temp.y4 += 1;
      break;
    }
    }
    temp.state = TetrominoState::LEFT;
    break;
  }
  case TetrominoState::TOP: {
    switch (this->type) {
    case TetrominoType::I: {
      temp.x1 -= 1;
      temp.y1 += 2;
      temp.y2 += 1;
      temp.x3 += 1;
      temp.x4 += 2;
      temp.y4 -= 1;
      break;
    }
    case TetrominoType::J: {
      temp.x1 -= 1;
      temp.y2 += 1;
      temp.x3 += 1;
      temp.x4 += 2;
      temp.y4 -= 1;
      break;
    }
    case TetrominoType::L: {
      temp.x1 -= 1;
      temp.y1 += 2;
      temp.y2 += 1;
      temp.x3 += 1;
      temp.y4 -= 1;
      break;
    }
    case TetrominoType::O: {
      break;
    }
    case TetrominoType::S: {
      temp.y1 += 2;
      temp.x2 += 1;
      temp.y2 += 1;
      temp.x4 += 1;
      temp.y4 -= 1;
      break;
    }
    case TetrominoType::T: {
      temp.x1 -= 1;
      temp.y1 += 1;
      temp.x2 -= 1;
      temp.y2 -= 1;
      temp.x4 += 1;
      temp.y4 -= 1;
      break;
    }
    case TetrominoType::Z: {
      temp.x1 -= 1;
      temp.y1 += 1;
      temp.x3 += 1;
      temp.y3 += 1;
      temp.x4 += 2;
      break;
    }
    }
    temp.state = TetrominoState::RIGHT;
    break;
  }
  case TetrominoState::LEFT: {
    switch (this->type) {
    case TetrominoType::I: {
      temp.x1 += 2;
      temp.y1 += 1;
      temp.x2 += 1;
      temp.y3 -= 1;
      temp.x4 -= 1;
      temp.y4 -= 2;
      break;
    }
    case TetrominoType::J: {
      temp.y1 += 1;
      temp.x2 += 1;
      temp.y3 -= 1;
      temp.x4 -= 1;
      temp.y4 -= 2;
      break;
    }
    case TetrominoType::L: {
      temp.x1 += 2;
      temp.y1 += 1;
      temp.x2 += 1;
      temp.y3 -= 1;
      temp.x4 -= 1;
      break;
    }
    case TetrominoType::O: {
      break;
    }
    case TetrominoType::S: {
      temp.x1 += 2;
      temp.x2 += 1;
      temp.y2 -= 1;
      temp.x4 -= 1;
      temp.y4 -= 1;
      break;
    }
    case TetrominoType::T: {
      temp.x1 += 1;
      temp.y1 += 1;
      temp.x2 -= 1;
      temp.y2 += 1;
      temp.x4 -= 1;
      temp.y4 -= 1;
      break;
    }
    case TetrominoType::Z: {
      temp.x1 += 1;
      temp.y1 += 1;
      temp.x3 += 1;
      temp.y3 -= 1;
      temp.y4 -= 2;
      break;
    }
    }
    temp.state = TetrominoState::TOP;
    break;
  }
  case TetrominoState::RIGHT: {
    switch (this->type) {
    case TetrominoType::I: {
      temp.x1 -= 2;
      temp.y1 -= 1;
      temp.x2 -= 1;
      temp.y3 += 1;
      temp.x4 += 1;
      temp.y4 += 2;
      break;
    }
    case TetrominoType::J: {
      temp.y1 -= 1;
      temp.x2 -= 1;
      temp.y3 += 1;
      temp.x4 += 1;
      temp.y4 += 2;
      break;
    }
    case TetrominoType::L: {
      temp.x1 -= 2;
      temp.y1 -= 1;
      temp.x2 -= 1;
      temp.y3 += 1;
      temp.x4 += 1;
      break;
    }
    case TetrominoType::O: {
      break;
    }
    case TetrominoType::S: {
      temp.x1 -= 2;
      temp.x2 -= 1;
      temp.y2 += 1;
      temp.x4 += 1;
      temp.y4 += 1;
      break;
    }
    case TetrominoType::T: {
      temp.x1 -= 1;
      temp.y1 -= 1;
      temp.x2 += 1;
      temp.y2 -= 1;
      temp.x4 += 1;
      temp.y4 += 1;
      break;
    }
    case TetrominoType::Z: {
      temp.x1 -= 1;
      temp.y1 -= 1;
      temp.x3 -= 1;
      temp.y3 += 1;
      temp.y4 += 2;
      break;
    }
    }
    temp.state = TetrominoState::BOTTOM;
    break;
  }
  }
  return temp;
}
