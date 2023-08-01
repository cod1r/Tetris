#include "Tetris.h"
#include "Renderer.h"
#include "Tetromino.h"
#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <cassert>
#include <chrono>
#include <climits>
#include <cstdint>
#include <experimental/simd>
#include <fstream>
#include <iostream>
#include <random>
#include <set>
#include <stdexcept>
#include <string>
#include <vector>

static constexpr std::array<double, 15> LevelSpeed{
    {1.0, 0.79300, 0.6178, 0.47273, 0.35520, 0.26200,
     0.18968, 0.13473, 0.09388, 0.06415, 0.04298, 0.02822,
     0.01815, 0.01144, 0.00706}};
static const std::array<Tetromino, 7> Tetrominos = {{
      Tetromino(TetrominoType::I, 3, 1, 4, 1, 5, 1, 6, 1, 173, 216, 230),
      Tetromino(TetrominoType::L, 2, 1, 3, 1, 4, 1, 4, 0, 0, 0, 139),
      Tetromino(TetrominoType::J, 2, 0, 2, 1, 3, 1, 4, 1, 240, 165, 0),
      Tetromino(TetrominoType::O, 4, 0, 5, 0, 4, 1, 5, 1, 255, 255, 0),
      Tetromino(TetrominoType::T, 2, 1, 3, 0, 3, 1, 4, 1, 128, 0, 128),
      Tetromino(TetrominoType::S, 2, 1, 3, 1, 3, 0, 4, 0, 0, 255, 0),
      Tetromino(TetrominoType::Z, 2, 0, 3, 0, 3, 1, 4, 1, 255, 0, 0),
  }};

Tetris::Tetris() {
  generate_sequence();
  current_tetromino = Tetromino(
      Tetrominos[sequence.at(sequence_index)]);
  renderer.render_tetromino(current_tetromino);
}

bool Tetris::check_fill() {
  if (playfield[current_tetromino.y1][current_tetromino.x1] != -1 ||
      playfield[current_tetromino.y2][current_tetromino.x2] != -1 ||
      playfield[current_tetromino.y3][current_tetromino.x3] != -1 ||
      playfield[current_tetromino.y4][current_tetromino.x4] != -1) {
    return false;
  }
  return true;
}

void Tetris::rotate() {
  Tetromino temp = current_tetromino.rotate();
  if (temp.y1 < 0 || temp.y2 < 0 || temp.y3 < 0 || temp.y4 < 0 || temp.x1 < 0 ||
      temp.x2 < 0 || temp.x3 < 0 || temp.x4 < 0 ||
      temp.y1 >= TETRIS_PLAYFIELD_HEIGHT ||
      temp.y2 >= TETRIS_PLAYFIELD_HEIGHT ||
      temp.y3 >= TETRIS_PLAYFIELD_HEIGHT ||
      temp.y4 >= TETRIS_PLAYFIELD_HEIGHT ||
      temp.x1 >= TETRIS_PLAYFIELD_WIDTH ||
      temp.x2 >= TETRIS_PLAYFIELD_WIDTH ||
      temp.x3 >= TETRIS_PLAYFIELD_WIDTH ||
      temp.x4 >= TETRIS_PLAYFIELD_WIDTH) {
    return;
  }
  if (playfield[temp.y1][temp.x1] != -1 ||
      playfield[temp.y2][temp.x2] != -1 ||
      playfield[temp.y3][temp.x3] != -1 ||
      playfield[temp.y4][temp.x4] != -1) {
    return;
  }
  current_tetromino = temp;
}

void Tetris::hard_drop() {
  int min_add = INT_MAX;

  min_add = SDL_min(min_add, TETRIS_PLAYFIELD_HEIGHT - 1 -
                                 Tetris::current_tetromino.y1);
  for (int Y = TETRIS_PLAYFIELD_HEIGHT - 1; Y > Tetris::current_tetromino.y1;
       --Y) {
    if (Tetris::playfield[Y][Tetris::current_tetromino.x1] != -1) {
      min_add = SDL_min(min_add, Y - 1 - Tetris::current_tetromino.y1);
    }
  }

  min_add = SDL_min(min_add, TETRIS_PLAYFIELD_HEIGHT - 1 -
                                 Tetris::current_tetromino.y2);
  for (int Y = TETRIS_PLAYFIELD_HEIGHT - 1; Y > Tetris::current_tetromino.y2;
       --Y) {
    if (Tetris::playfield[Y][Tetris::current_tetromino.x2] != -1) {
      min_add = SDL_min(min_add, Y - 1 - Tetris::current_tetromino.y2);
    }
  }

  min_add = SDL_min(min_add, TETRIS_PLAYFIELD_HEIGHT - 1 -
                                 Tetris::current_tetromino.y3);
  for (int Y = TETRIS_PLAYFIELD_HEIGHT - 1; Y > Tetris::current_tetromino.y3;
       --Y) {
    if (Tetris::playfield[Y][Tetris::current_tetromino.x3] != -1) {
      min_add = SDL_min(min_add, Y - 1 - Tetris::current_tetromino.y3);
    }
  }

  min_add = SDL_min(min_add, TETRIS_PLAYFIELD_HEIGHT - 1 -
                                 Tetris::current_tetromino.y4);
  for (int Y = TETRIS_PLAYFIELD_HEIGHT - 1; Y > Tetris::current_tetromino.y4;
       --Y) {
    if (Tetris::playfield[Y][Tetris::current_tetromino.x4] != -1) {
      min_add = SDL_min(min_add, Y - 1 - Tetris::current_tetromino.y4);
    }
  }

  Tetris::current_tetromino.y1 += min_add;
  Tetris::current_tetromino.y2 += min_add;
  Tetris::current_tetromino.y3 += min_add;
  Tetris::current_tetromino.y4 += min_add;
}

void Tetris::lock() {
  playfield[current_tetromino.y1]
                   [current_tetromino.x1] =
                       sequence.at(sequence_index) + 1;
  playfield[current_tetromino.y2]
                   [current_tetromino.x2] =
                       sequence.at(sequence_index) + 1;
  playfield[current_tetromino.y3]
                   [current_tetromino.x3] =
                       sequence.at(sequence_index) + 1;
  playfield[current_tetromino.y4]
                   [current_tetromino.x4] =
                       sequence.at(sequence_index) + 1;

  ++sequence_index;
  if (sequence_index == 7) {
    generate_sequence();
    sequence_index = 0;
  }
  current_tetromino = Tetromino(
      Tetrominos.at(sequence.at(sequence_index)));
  renderer.render_tetromino(current_tetromino);
}

bool Tetris::check_collision() {
  if (Tetris::current_tetromino.y1 + 1 == TETRIS_PLAYFIELD_HEIGHT ||
      (Tetris::current_tetromino.y1 + 1 < TETRIS_PLAYFIELD_HEIGHT &&
       Tetris::playfield[Tetris::current_tetromino.y1 + 1]
                        [Tetris::current_tetromino.x1] != -1)) {
    return true;
  }
  if (Tetris::current_tetromino.y2 + 1 == TETRIS_PLAYFIELD_HEIGHT ||
      (Tetris::current_tetromino.y2 + 1 < TETRIS_PLAYFIELD_HEIGHT &&
       Tetris::playfield[Tetris::current_tetromino.y2 + 1]
                        [Tetris::current_tetromino.x2] != -1)) {
    return true;
  }
  if (Tetris::current_tetromino.y3 + 1 == TETRIS_PLAYFIELD_HEIGHT ||
      (Tetris::current_tetromino.y3 + 1 < TETRIS_PLAYFIELD_HEIGHT &&
       Tetris::playfield[Tetris::current_tetromino.y3 + 1]
                        [Tetris::current_tetromino.x3] != -1)) {
    return true;
  }
  if (Tetris::current_tetromino.y4 + 1 == TETRIS_PLAYFIELD_HEIGHT ||
      (Tetris::current_tetromino.y4 + 1 < TETRIS_PLAYFIELD_HEIGHT &&
       Tetris::playfield[Tetris::current_tetromino.y4 + 1]
                        [Tetris::current_tetromino.x4] != -1)) {
    return true;
  }
  return false;
}

void Tetris::check_line_clear() {
  std::vector<int32_t> rows_deleted;
  for (int32_t ROW = 0; ROW < TETRIS_PLAYFIELD_HEIGHT; ++ROW) {
    int32_t counter = 0;
    for (int32_t COL = 0; COL < TETRIS_PLAYFIELD_WIDTH; ++COL) {
      if (playfield[ROW][COL] != -1) {
        ++counter;
      }
    }
    if (counter == TETRIS_PLAYFIELD_WIDTH) {
      rows_deleted.push_back(ROW);
      renderer.delete_row(ROW);
    }
  }
  if (!rows_deleted.empty()) {
    text_dump("BEFORE\n");
  }
  for (int32_t &DEL_ROW : rows_deleted) {
    for (int32_t COL = 0; COL < TETRIS_PLAYFIELD_WIDTH; ++COL) {
      playfield[DEL_ROW][COL] = -1;
    }
    for (int32_t Y = DEL_ROW; Y > 0; --Y) {
      for (int32_t X = 0; X < TETRIS_PLAYFIELD_WIDTH; ++X) {
        playfield[Y][X] = playfield[Y - 1][X];
      }
    }
    renderer.update_playfield(DEL_ROW);
  }
  if (!rows_deleted.empty()) {
    text_dump("AFTER\n");
  }
  if (lines_cleared == LINE_CLEAR_LEVELUP_AMOUNT) {
    ++level;
    lines_cleared = 0;
  }
}

void Tetris::generate_sequence() {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> distrib(0, 6);
  std::set<int32_t> seen;
  int32_t temp_sequence_index = 0;
  while (seen.size() < 7) {
    int32_t a = distrib(gen);
    if (!seen.contains(a)) {
      seen.insert(a);
      sequence.at(temp_sequence_index++) = a;
void Tetris::text_dump(const std::string& msg) const {
  std::cout << msg;
  for (int32_t row = 0; row < TETRIS_PLAYFIELD_HEIGHT; ++row) {
    for (int32_t col = 0; col < TETRIS_PLAYFIELD_WIDTH; ++col) {
      std::cout << (char)(playfield[row][col] == -1 ? '.' : '0'+playfield[row][col]);
    }
    std::cout << '\n';
  }
  std::cout << '\n';
}

bool Tetris::check_left() {
  if (current_tetromino.x1 - 1 < 0 ||
      (current_tetromino.x1 - 1 >= 0 &&
       playfield[current_tetromino.y1]
                      [current_tetromino.x1 - 1] != -1))
    return false;
  if (current_tetromino.x2 - 1 < 0 ||
      (current_tetromino.x2 - 1 >= 0 &&
       playfield[current_tetromino.y2]
                      [current_tetromino.x2 - 1] != -1))
    return false;
  if (current_tetromino.x3 - 1 < 0 ||
      (current_tetromino.x3 - 1 >= 0 &&
       playfield[current_tetromino.y3]
                      [current_tetromino.x3 - 1] != -1))
    return false;
  if (current_tetromino.x4 - 1 < 0 ||
      (current_tetromino.x4 - 1 >= 0 &&
       playfield[current_tetromino.y4]
                      [current_tetromino.x4 - 1] != -1))
    return false;
  return true;
}

bool Tetris::check_right() {
  if (current_tetromino.x1 + 1 == TETRIS_PLAYFIELD_WIDTH ||
      (current_tetromino.x1 + 1 < TETRIS_PLAYFIELD_WIDTH &&
       playfield[current_tetromino.y1]
                      [current_tetromino.x1 + 1] != -1))
    return false;
  if (current_tetromino.x2 + 1 == TETRIS_PLAYFIELD_WIDTH ||
      (current_tetromino.x2 + 1 < TETRIS_PLAYFIELD_WIDTH &&
       playfield[current_tetromino.y2]
                      [current_tetromino.x2 + 1] != -1))
    return false;
  if (current_tetromino.x3 + 1 == TETRIS_PLAYFIELD_WIDTH ||
      (current_tetromino.x3 + 1 < TETRIS_PLAYFIELD_WIDTH &&
       playfield[current_tetromino.y3]
                      [current_tetromino.x3 + 1] != -1))
    return false;
  if (current_tetromino.x4 + 1 == TETRIS_PLAYFIELD_WIDTH ||
      (current_tetromino.x4 + 1 < TETRIS_PLAYFIELD_WIDTH &&
       playfield[current_tetromino.y4]
                      [current_tetromino.x4 + 1] != -1))
    return false;
  return true;
}

bool Tetris::check_down() {
  if (current_tetromino.y1 + 1 == TETRIS_PLAYFIELD_HEIGHT ||
      (current_tetromino.y1 + 1 < TETRIS_PLAYFIELD_HEIGHT &&
       playfield[current_tetromino.y1 + 1]
                      [current_tetromino.x1] != -1)) {
    return false;
  }
  if (current_tetromino.y2 + 1 == TETRIS_PLAYFIELD_HEIGHT ||
      (current_tetromino.y2 + 1 < TETRIS_PLAYFIELD_HEIGHT &&
       playfield[current_tetromino.y2 + 1]
                      [current_tetromino.x2] != -1)) {
    return false;
  }
  if (current_tetromino.y3 + 1 == TETRIS_PLAYFIELD_HEIGHT ||
      (current_tetromino.y3 + 1 < TETRIS_PLAYFIELD_HEIGHT &&
       playfield[current_tetromino.y3 + 1]
                      [current_tetromino.x3] != -1)) {
    return false;
  }
  if (current_tetromino.y4 + 1 == TETRIS_PLAYFIELD_HEIGHT ||
      (current_tetromino.y4 + 1 < TETRIS_PLAYFIELD_HEIGHT &&
       playfield[current_tetromino.y4 + 1]
                      [current_tetromino.x4] != -1)) {
    return false;
  }
  return true;
}

void Tetris::loop() {
  SDL_Event e;
  std::chrono::time_point<std::chrono::system_clock> prev =
      std::chrono::system_clock::now();
  std::chrono::time_point<std::chrono::system_clock> last_input_time_down =
      std::chrono::system_clock::now();
  std::chrono::time_point<std::chrono::system_clock> last_input_time_left =
      std::chrono::system_clock::now();
  std::chrono::time_point<std::chrono::system_clock> last_input_time_right =
      std::chrono::system_clock::now();
  std::chrono::time_point<std::chrono::system_clock> last_input_time_up =
      std::chrono::system_clock::now();
  std::chrono::time_point<std::chrono::system_clock> collide_time;
  bool collided = false;
  bool paused = false;
  while (level < MAX_LEVELS && check_fill()) {
    while (SDL_PollEvent(&e)) {
      switch (e.type) {
      case SDL_KEYDOWN:
        if (e.key.keysym.sym == SDLK_q) {
          return;
        } else if (e.key.keysym.sym == SDLK_SPACE) {
          space = true;
        } else if (e.key.keysym.sym == SDLK_UP) {
          up = true;
        } else if (e.key.keysym.sym == SDLK_RIGHT) {
          right = true;
        } else if (e.key.keysym.sym == SDLK_LEFT) {
          left = true;
        } else if (e.key.keysym.sym == SDLK_DOWN) {
          down = true;
        } else if (e.key.keysym.sym == SDLK_p) {
          paused = !paused;
        }
        break;
      case SDL_KEYUP:
        if (e.key.keysym.sym == SDLK_RIGHT) {
          right = false;
        } else if (e.key.keysym.sym == SDLK_LEFT) {
          left = false;
        } else if (e.key.keysym.sym == SDLK_DOWN) {
          down = false;
        } else if (e.key.keysym.sym == SDLK_UP) {
          up = false;
        } else if (e.key.keysym.sym == SDLK_SPACE) {
          space = false;
        }
        break;
      default:
        break;
      }
    }
    if (paused) {
      continue;
    }
    if (up && std::chrono::duration<double>(
                        std::chrono::system_clock::now() - last_input_time_up)
                            .count() >= ROTATE_SPEED) {
      last_input_time_up = std::chrono::system_clock::now();
      rotate();
    }
    if (left &&
        std::chrono::duration<double>(std::chrono::system_clock::now() -
                                      last_input_time_left)
                .count() >= SOFT_MOVE_SPEED &&
        check_left()) {
      last_input_time_left = std::chrono::system_clock::now();
      --current_tetromino.x1;
      --current_tetromino.x2;
      --current_tetromino.x3;
      --current_tetromino.x4;
    }
    if (right &&
        std::chrono::duration<double>(std::chrono::system_clock::now() -
                                      last_input_time_right)
                .count() >= SOFT_MOVE_SPEED &&
        check_right()) {
      last_input_time_right = std::chrono::system_clock::now();
      ++current_tetromino.x1;
      ++current_tetromino.x2;
      ++current_tetromino.x3;
      ++current_tetromino.x4;
    }
    if (down &&
        std::chrono::duration<double>(std::chrono::system_clock::now() -
                                      last_input_time_down)
                .count() >= SOFT_MOVE_SPEED &&
        check_down()) {
      last_input_time_down = std::chrono::system_clock::now();
      ++current_tetromino.y1;
      ++current_tetromino.y2;
      ++current_tetromino.y3;
      ++current_tetromino.y4;
    }

    if (space) {
      hard_drop();
    }

    std::chrono::time_point<std::chrono::system_clock> curr =
        std::chrono::system_clock::now();
    bool right_time = std::chrono::duration<double>(curr - prev).count() >=
                      LevelSpeed.at(level);

    if (right_time && !down && check_down()) {
      prev = std::chrono::system_clock::now();
      ++current_tetromino.y1;
      ++current_tetromino.y2;
      ++current_tetromino.y3;
      ++current_tetromino.y4;
    }

    if (check_collision()) {
      if (!collided) {
        collide_time = std::chrono::system_clock::now();
      }
      collided = true;
    } else {
      collided = false;
    }
    if (collided && std::chrono::duration<double>(
                        std::chrono::system_clock::now() - collide_time)
                            .count() >= LOCK_DELAY) {
      collide_time = std::chrono::system_clock::now();
      lock();
      collided = false;
    }
    check_line_clear();
    renderer.update_tetromino(current_tetromino);
    renderer.render();
  }
}
