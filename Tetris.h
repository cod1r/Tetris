#include <SDL2/SDL.h>
#include <vector>
#ifndef _TETRIS_H
#define _TETRIS_H
class Tetris {
	std::vector<std::vector<int>> playfield;
	SDL_Window *window = nullptr;
	int speed = 1000;
	int spawnTetromino();
	int xoff = 0;
	int yoff = 0;
	bool left = false;
	bool right = false;
	bool down = false;
	bool up = false;
	void draw();
	public:
		void loop();
		Tetris(int, int);
		~Tetris();
};
#endif
