#include <SDL2/SDL.h>
#include <string>
#include <iostream>
#include "Tetris.h"
#define WIDTH 400
#define HEIGHT 600

int main(int argc, char *argv[])
{
	Tetris game = Tetris(WIDTH, HEIGHT);
	game.loop();
	return 0;
}
