#include <SDL2/SDL.h>
#include <vector>
#ifndef _TETRIS_H
#define _TETRIS_H
class Tetris {
	std::vector<std::vector<int>> playfield;
	SDL_Window *window = nullptr;
	int speed = 1000;
	int spawnTetromino();
	void loop();
	void draw();
	public:
		Tetris(int, int);
		~Tetris();
};
#endif
