#include "Tetromino.h"
#include <cstdint>
#include <iostream>

Tetromino::Tetromino(TetrominoType type, int32_t x1, int32_t y1, int32_t x2,
                     int32_t y2, int32_t x3, int32_t y3, int32_t x4, int32_t y4,
                     uint8_t red, uint8_t green, uint8_t blue) 
    : type{type}
    , x1{x1}
    , y1{y1}
    , x2{x2}
    , y2{y2}
    , x3{x3}
    , y3{y3}
    , x4{x4}
    , y4{y4}
    , red{red}
    , blue{blue}
    , green{green}
    , state{TetrominoState::BOTTOM}
{
}

Tetromino::Tetromino(const Tetromino &t) = default;

Tetromino &Tetromino::operator=(const Tetromino &t) {
  type = t.type;
  x1 = t.x1;
  x2 = t.x2;
  x3 = t.x3;
  x4 = t.x4;
  y1 = t.y1;
  y2 = t.y2;
  y3 = t.y3;
  y4 = t.y4;
  red = t.red;
  green = t.green;
  blue = t.blue;
  state = t.state;
  return *this;
}

Tetromino::Tetromino() = default;

Tetromino Tetromino::rotate() {
  Tetromino temp = *this;
  switch (state) {
  case TetrominoState::BOTTOM: {
    switch (type) {
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
    switch (type) {
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
    switch (type) {
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
    switch (type) {
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
