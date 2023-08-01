#include <SDL2/SDL.h>
#include <array>
#include <cstdint>
#include <vector>
#include "Renderer.h"
#include "Tetromino.h"
#ifndef _TETRIS_H
#define _TETRIS_H
struct Tetris {
  static const int32_t TETRIS_PLAYFIELD_WIDTH = 10;
  static const int32_t TETRIS_PLAYFIELD_HEIGHT = 20;
  std::array<std::array<int32_t, TETRIS_PLAYFIELD_WIDTH>,
             TETRIS_PLAYFIELD_HEIGHT>
      playfield = {{
          {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
          {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
          {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
          {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
          {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
          {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
          {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
          {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
          {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
          {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
          {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
          {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
          {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
          {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
          {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
          {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
          {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
          {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
          {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
          {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
      }};
  Renderer renderer{};
  std::array<int32_t, 7> sequence;
  Tetromino current_tetromino;
  const int32_t LINE_CLEAR_LEVELUP_AMOUNT = 10;
  const int32_t MAX_LEVELS = 15;
  int32_t sequence_index = 0;
  int32_t level = 0;
  int32_t lines_cleared = 0;
  const double ROTATE_SPEED = .15;
  const double SOFT_MOVE_SPEED = .15;
  const double LOCK_DELAY = .5;
  bool left = false;
  bool right = false;
  bool down = false;
  bool up = false;
  bool space = false;
  void hard_drop();
  void rotate();
  bool check_left();
  bool check_right();
  bool check_down();
  bool check_collision();
  bool check_fill();
  void lock();
  void check_line_clear();
  void generate_sequence();
  void loop();
  Tetris();
};
#endif
