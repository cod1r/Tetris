#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <string>
#include <iostream>
#include "Tetris.h"

int main(int argc, char *argv[])
{
	Tetris game = Tetris();
	game.loop();
	return 0;
}
