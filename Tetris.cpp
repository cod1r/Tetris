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

Tetris::Tetris() {
  renderer = new Renderer();
  this->Tetrominos = {{
      new Tetromino(TetrominoType::I, 3, 1, 4, 1, 5, 1, 6, 1, 173, 216, 230),
      new Tetromino(TetrominoType::L, 2, 1, 3, 1, 4, 1, 4, 0, 0, 0, 139),
      new Tetromino(TetrominoType::J, 2, 0, 2, 1, 3, 1, 4, 1, 240, 165, 0),
      new Tetromino(TetrominoType::O, 4, 0, 5, 0, 4, 1, 5, 1, 255, 255, 0),
      new Tetromino(TetrominoType::T, 2, 1, 3, 0, 3, 1, 4, 1, 128, 0, 128),
      new Tetromino(TetrominoType::S, 2, 1, 3, 1, 3, 0, 4, 0, 0, 255, 0),
      new Tetromino(TetrominoType::Z, 2, 0, 3, 0, 3, 1, 4, 1, 255, 0, 0),
  }};
  this->LevelSpeed = {{1.0, 0.79300, 0.6178, 0.47273, 0.35520, 0.26200,
                         0.18968, 0.13473, 0.09388, 0.06415, 0.04298, 0.02822,
                         0.01815, 0.01144, 0.00706}};
  this->generate_sequence();
  this->current_tetromino = new Tetromino(
      *this->Tetrominos[this->sequence.at(this->sequence_index)]);
  this->renderer->render_tetromino(*this->current_tetromino);
}

bool Tetris::check_fill() {
  if (this->playfield[current_tetromino->y1][current_tetromino->x1] != -1 ||
      this->playfield[current_tetromino->y2][current_tetromino->x2] != -1 ||
      this->playfield[current_tetromino->y3][current_tetromino->x3] != -1 ||
      this->playfield[current_tetromino->y4][current_tetromino->x4] != -1) {
    return false;
  }
  return true;
}

void Tetris::rotate() {
  Tetromino temp = this->current_tetromino->rotate();
  if (temp.y1 < 0 || temp.y2 < 0 || temp.y3 < 0 || temp.y4 < 0 || temp.x1 < 0 ||
      temp.x2 < 0 || temp.x3 < 0 || temp.x4 < 0 ||
      temp.y1 >= this->TETRIS_PLAYFIELD_HEIGHT ||
      temp.y2 >= this->TETRIS_PLAYFIELD_HEIGHT ||
      temp.y3 >= this->TETRIS_PLAYFIELD_HEIGHT ||
      temp.y4 >= this->TETRIS_PLAYFIELD_HEIGHT ||
      temp.x1 >= this->TETRIS_PLAYFIELD_WIDTH ||
      temp.x2 >= this->TETRIS_PLAYFIELD_WIDTH ||
      temp.x3 >= this->TETRIS_PLAYFIELD_WIDTH ||
      temp.x4 >= this->TETRIS_PLAYFIELD_WIDTH) {
    return;
  }
  if (this->playfield[temp.y1][temp.x1] != -1 ||
      this->playfield[temp.y2][temp.x2] != -1 ||
      this->playfield[temp.y3][temp.x3] != -1 ||
      this->playfield[temp.y4][temp.x4] != -1) {
    return;
  }
  *this->current_tetromino = temp;
}

void Tetris::hard_drop() {
  int min_add = INT_MAX;

  min_add = SDL_min(min_add, TETRIS_PLAYFIELD_HEIGHT - 1 -
                                 Tetris::current_tetromino->y1);
  for (int Y = TETRIS_PLAYFIELD_HEIGHT - 1; Y > Tetris::current_tetromino->y1;
       --Y) {
    if (Tetris::playfield[Y][Tetris::current_tetromino->x1] != -1) {
      min_add = SDL_min(min_add, Y - 1 - Tetris::current_tetromino->y1);
    }
  }

  min_add = SDL_min(min_add, TETRIS_PLAYFIELD_HEIGHT - 1 -
                                 Tetris::current_tetromino->y2);
  for (int Y = TETRIS_PLAYFIELD_HEIGHT - 1; Y > Tetris::current_tetromino->y2;
       --Y) {
    if (Tetris::playfield[Y][Tetris::current_tetromino->x2] != -1) {
      min_add = SDL_min(min_add, Y - 1 - Tetris::current_tetromino->y2);
    }
  }

  min_add = SDL_min(min_add, TETRIS_PLAYFIELD_HEIGHT - 1 -
                                 Tetris::current_tetromino->y3);
  for (int Y = TETRIS_PLAYFIELD_HEIGHT - 1; Y > Tetris::current_tetromino->y3;
       --Y) {
    if (Tetris::playfield[Y][Tetris::current_tetromino->x3] != -1) {
      min_add = SDL_min(min_add, Y - 1 - Tetris::current_tetromino->y3);
    }
  }

  min_add = SDL_min(min_add, TETRIS_PLAYFIELD_HEIGHT - 1 -
                                 Tetris::current_tetromino->y4);
  for (int Y = TETRIS_PLAYFIELD_HEIGHT - 1; Y > Tetris::current_tetromino->y4;
       --Y) {
    if (Tetris::playfield[Y][Tetris::current_tetromino->x4] != -1) {
      min_add = SDL_min(min_add, Y - 1 - Tetris::current_tetromino->y4);
    }
  }

  Tetris::current_tetromino->y1 += min_add;
  Tetris::current_tetromino->y2 += min_add;
  Tetris::current_tetromino->y3 += min_add;
  Tetris::current_tetromino->y4 += min_add;
}

void Tetris::lock() {
  this->playfield[this->current_tetromino->y1]
                   [this->current_tetromino->x1] =
                       this->sequence.at(this->sequence_index) + 1;
  this->playfield[this->current_tetromino->y2]
                   [this->current_tetromino->x2] =
                       this->sequence.at(this->sequence_index) + 1;
  this->playfield[this->current_tetromino->y3]
                   [this->current_tetromino->x3] =
                       this->sequence.at(this->sequence_index) + 1;
  this->playfield[this->current_tetromino->y4]
                   [this->current_tetromino->x4] =
                       this->sequence.at(this->sequence_index) + 1;

  ++this->sequence_index;
  if (this->sequence_index == 7) {
    this->generate_sequence();
    this->sequence_index = 0;
  }
  *this->current_tetromino = Tetromino(
      *this->Tetrominos.at(this->sequence.at(this->sequence_index)));
  this->renderer->render_tetromino(*this->current_tetromino);
}

bool Tetris::check_collision() {
  if (Tetris::current_tetromino->y1 + 1 == TETRIS_PLAYFIELD_HEIGHT ||
      (Tetris::current_tetromino->y1 + 1 < TETRIS_PLAYFIELD_HEIGHT &&
       Tetris::playfield[Tetris::current_tetromino->y1 + 1]
                        [Tetris::current_tetromino->x1] != -1)) {
    return true;
  }
  if (Tetris::current_tetromino->y2 + 1 == TETRIS_PLAYFIELD_HEIGHT ||
      (Tetris::current_tetromino->y2 + 1 < TETRIS_PLAYFIELD_HEIGHT &&
       Tetris::playfield[Tetris::current_tetromino->y2 + 1]
                        [Tetris::current_tetromino->x2] != -1)) {
    return true;
  }
  if (Tetris::current_tetromino->y3 + 1 == TETRIS_PLAYFIELD_HEIGHT ||
      (Tetris::current_tetromino->y3 + 1 < TETRIS_PLAYFIELD_HEIGHT &&
       Tetris::playfield[Tetris::current_tetromino->y3 + 1]
                        [Tetris::current_tetromino->x3] != -1)) {
    return true;
  }
  if (Tetris::current_tetromino->y4 + 1 == TETRIS_PLAYFIELD_HEIGHT ||
      (Tetris::current_tetromino->y4 + 1 < TETRIS_PLAYFIELD_HEIGHT &&
       Tetris::playfield[Tetris::current_tetromino->y4 + 1]
                        [Tetris::current_tetromino->x4] != -1)) {
    return true;
  }
  return false;
}

void Tetris::check_line_clear() {
  std::vector<int32_t> rows_deleted;
  for (int32_t ROW = 0; ROW < this->TETRIS_PLAYFIELD_HEIGHT; ++ROW) {
    int32_t counter = 0;
    for (int32_t COL = 0; COL < this->TETRIS_PLAYFIELD_WIDTH; ++COL) {
      if (this->playfield[ROW][COL] != -1) {
        ++counter;
      }
    }
    if (counter == this->TETRIS_PLAYFIELD_WIDTH) {
      rows_deleted.push_back(ROW);
      this->renderer->delete_row(ROW);
    }
  }
  if (!rows_deleted.empty()) {
    std::cerr << std::format("BEFORE\n");
    for (int32_t row = 0; row < this->TETRIS_PLAYFIELD_HEIGHT; ++row) {
      for (int32_t col = 0; col < this->TETRIS_PLAYFIELD_WIDTH; ++col) {
        std::cerr << std::format("{} ", this->playfield[row][col] == -1
            ? 0
            : this->playfield[row][col]);
      }
      std::cerr << std::endl;
    }
    std::cerr << std::endl;
  }
  for (int32_t &DEL_ROW : rows_deleted) {
    for (int32_t COL = 0; COL < this->TETRIS_PLAYFIELD_WIDTH; ++COL) {
      this->playfield[DEL_ROW][COL] = -1;
    }
    for (int32_t Y = DEL_ROW; Y > 0; --Y) {
      for (int32_t X = 0; X < this->TETRIS_PLAYFIELD_WIDTH; ++X) {
        this->playfield[Y][X] = this->playfield[Y - 1][X];
      }
    }
    this->renderer->update_playfield(DEL_ROW);
  }
  if (!rows_deleted.empty()) {
    std::cerr << std::format("AFTER\n");
    for (int32_t row = 0; row < this->TETRIS_PLAYFIELD_HEIGHT; ++row) {
      for (int32_t col = 0; col < this->TETRIS_PLAYFIELD_WIDTH; ++col) {
        std::cerr << std::format("{} ", this->playfield[row][col] == -1
            ? 0
            : this->playfield[row][col]);
      }
      std::cerr << std::endl;
    }
    std::cerr << std::endl;
  }
  if (this->lines_cleared == this->LINE_CLEAR_LEVELUP_AMOUNT) {
    ++this->level;
    this->lines_cleared = 0;
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
      this->sequence.at(temp_sequence_index++) = a;
    }
  }
}

bool Tetris::check_left() {
  if (this->current_tetromino->x1 - 1 < 0 ||
      (this->current_tetromino->x1 - 1 >= 0 &&
       this->playfield[this->current_tetromino->y1]
                      [this->current_tetromino->x1 - 1] != -1))
    return false;
  if (this->current_tetromino->x2 - 1 < 0 ||
      (this->current_tetromino->x2 - 1 >= 0 &&
       this->playfield[this->current_tetromino->y2]
                      [this->current_tetromino->x2 - 1] != -1))
    return false;
  if (this->current_tetromino->x3 - 1 < 0 ||
      (this->current_tetromino->x3 - 1 >= 0 &&
       this->playfield[this->current_tetromino->y3]
                      [this->current_tetromino->x3 - 1] != -1))
    return false;
  if (this->current_tetromino->x4 - 1 < 0 ||
      (this->current_tetromino->x4 - 1 >= 0 &&
       this->playfield[this->current_tetromino->y4]
                      [this->current_tetromino->x4 - 1] != -1))
    return false;
  return true;
}

bool Tetris::check_right() {
  if (this->current_tetromino->x1 + 1 == TETRIS_PLAYFIELD_WIDTH ||
      (this->current_tetromino->x1 + 1 < TETRIS_PLAYFIELD_WIDTH &&
       this->playfield[this->current_tetromino->y1]
                      [this->current_tetromino->x1 + 1] != -1))
    return false;
  if (this->current_tetromino->x2 + 1 == TETRIS_PLAYFIELD_WIDTH ||
      (this->current_tetromino->x2 + 1 < TETRIS_PLAYFIELD_WIDTH &&
       this->playfield[this->current_tetromino->y2]
                      [this->current_tetromino->x2 + 1] != -1))
    return false;
  if (this->current_tetromino->x3 + 1 == TETRIS_PLAYFIELD_WIDTH ||
      (this->current_tetromino->x3 + 1 < TETRIS_PLAYFIELD_WIDTH &&
       this->playfield[this->current_tetromino->y3]
                      [this->current_tetromino->x3 + 1] != -1))
    return false;
  if (this->current_tetromino->x4 + 1 == TETRIS_PLAYFIELD_WIDTH ||
      (this->current_tetromino->x4 + 1 < TETRIS_PLAYFIELD_WIDTH &&
       this->playfield[this->current_tetromino->y4]
                      [this->current_tetromino->x4 + 1] != -1))
    return false;
  return true;
}

bool Tetris::check_down() {
  if (this->current_tetromino->y1 + 1 == TETRIS_PLAYFIELD_HEIGHT ||
      (this->current_tetromino->y1 + 1 < TETRIS_PLAYFIELD_HEIGHT &&
       this->playfield[this->current_tetromino->y1 + 1]
                      [this->current_tetromino->x1] != -1)) {
    return false;
  }
  if (this->current_tetromino->y2 + 1 == TETRIS_PLAYFIELD_HEIGHT ||
      (this->current_tetromino->y2 + 1 < TETRIS_PLAYFIELD_HEIGHT &&
       this->playfield[this->current_tetromino->y2 + 1]
                      [this->current_tetromino->x2] != -1)) {
    return false;
  }
  if (this->current_tetromino->y3 + 1 == TETRIS_PLAYFIELD_HEIGHT ||
      (this->current_tetromino->y3 + 1 < TETRIS_PLAYFIELD_HEIGHT &&
       this->playfield[this->current_tetromino->y3 + 1]
                      [this->current_tetromino->x3] != -1)) {
    return false;
  }
  if (this->current_tetromino->y4 + 1 == TETRIS_PLAYFIELD_HEIGHT ||
      (this->current_tetromino->y4 + 1 < TETRIS_PLAYFIELD_HEIGHT &&
       this->playfield[this->current_tetromino->y4 + 1]
                      [this->current_tetromino->x4] != -1)) {
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
  while (this->level < this->MAX_LEVELS && this->check_fill()) {
    while (SDL_PollEvent(&e)) {
      switch (e.type) {
      case SDL_KEYDOWN:
        if (e.key.keysym.sym == SDLK_q) {
          return;
        } else if (e.key.keysym.sym == SDLK_SPACE) {
          this->space = true;
        } else if (e.key.keysym.sym == SDLK_UP) {
          this->up = true;
        } else if (e.key.keysym.sym == SDLK_RIGHT) {
          this->right = true;
        } else if (e.key.keysym.sym == SDLK_LEFT) {
          this->left = true;
        } else if (e.key.keysym.sym == SDLK_DOWN) {
          this->down = true;
        } else if (e.key.keysym.sym == SDLK_p) {
          paused = !paused;
        }
        break;
      case SDL_KEYUP:
        if (e.key.keysym.sym == SDLK_RIGHT) {
          this->right = false;
        } else if (e.key.keysym.sym == SDLK_LEFT) {
          this->left = false;
        } else if (e.key.keysym.sym == SDLK_DOWN) {
          this->down = false;
        } else if (e.key.keysym.sym == SDLK_UP) {
          this->up = false;
        } else if (e.key.keysym.sym == SDLK_SPACE) {
          this->space = false;
        }
        break;
      default:
        break;
      }
    }
    if (paused) {
      continue;
    }
    if (this->up && std::chrono::duration<double>(
                        std::chrono::system_clock::now() - last_input_time_up)
                            .count() >= this->ROTATE_SPEED) {
      last_input_time_up = std::chrono::system_clock::now();
      this->rotate();
    }
    if (this->left &&
        std::chrono::duration<double>(std::chrono::system_clock::now() -
                                      last_input_time_left)
                .count() >= this->SOFT_MOVE_SPEED &&
        this->check_left()) {
      last_input_time_left = std::chrono::system_clock::now();
      --this->current_tetromino->x1;
      --this->current_tetromino->x2;
      --this->current_tetromino->x3;
      --this->current_tetromino->x4;
    }
    if (this->right &&
        std::chrono::duration<double>(std::chrono::system_clock::now() -
                                      last_input_time_right)
                .count() >= this->SOFT_MOVE_SPEED &&
        this->check_right()) {
      last_input_time_right = std::chrono::system_clock::now();
      ++this->current_tetromino->x1;
      ++this->current_tetromino->x2;
      ++this->current_tetromino->x3;
      ++this->current_tetromino->x4;
    }
    if (this->down &&
        std::chrono::duration<double>(std::chrono::system_clock::now() -
                                      last_input_time_down)
                .count() >= this->SOFT_MOVE_SPEED &&
        this->check_down()) {
      last_input_time_down = std::chrono::system_clock::now();
      ++this->current_tetromino->y1;
      ++this->current_tetromino->y2;
      ++this->current_tetromino->y3;
      ++this->current_tetromino->y4;
    }

    if (this->space) {
      this->hard_drop();
    }

    std::chrono::time_point<std::chrono::system_clock> curr =
        std::chrono::system_clock::now();
    bool right_time = std::chrono::duration<double>(curr - prev).count() >=
                      this->LevelSpeed.at(this->level);

    if (right_time && !this->down && this->check_down()) {
      prev = std::chrono::system_clock::now();
      ++this->current_tetromino->y1;
      ++this->current_tetromino->y2;
      ++this->current_tetromino->y3;
      ++this->current_tetromino->y4;
    }

    if (this->check_collision()) {
      if (!collided) {
        collide_time = std::chrono::system_clock::now();
      }
      collided = true;
    } else {
      collided = false;
    }
    if (collided && std::chrono::duration<double>(
                        std::chrono::system_clock::now() - collide_time)
                            .count() >= this->LOCK_DELAY) {
      collide_time = std::chrono::system_clock::now();
      this->lock();
      collided = false;
    }
    this->check_line_clear();
    renderer->update_tetromino(*this->current_tetromino);
    renderer->render();
  }
}
