#include <SDL2/SDL.h>
#include <vector>
#ifndef _TETRIS_H
#define _TETRIS_H
class Tetris {
	std::vector<std::vector<int>> playfield;
	SDL_Window *window = nullptr;
	int TICK = 1000;
	int spawnTetromino();
	public:
		Tetris(int, int);
		void loop();
		~Tetris();
};
#endif
