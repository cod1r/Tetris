#include "Tetris.h"
#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <cstdint>
#include <iostream>
#include <string>

int main() {
  Tetris game = Tetris();
  game.loop();
  return 0;
}
