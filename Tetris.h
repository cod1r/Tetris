#include <SDL2/SDL.h>
#include <vector>
#include <tuple>
#include <array>
#ifndef _TETRIS_H
#define _TETRIS_H
class Tetris {
	typedef std::pair<std::tuple<int, int, int>,
					std::array<std::pair<int, int>, 4>> Tetromino;
	enum TetrominoType {
		I,
		J,
		L,
		O,
		S,
		T,
		Z
	};
	std::vector<std::vector<int>> playfield;
	SDL_Window *window = nullptr;
	// I J L O S T Z
	std::array<Tetromino, 7> Tetrominos;
	Tetromino currentTetromino;
	const int blocksize = 25;
	const int TETRIS_PLAYFIELD_WIDTH = 10;
	const int TETRIS_PLAYFIELD_HEIGHT = 20;
	const int TETROMINO_SIZE = 4;
	int xleftborder;
	int xrightborder;
	int gravity = 1000;
	bool left = false;
	bool right = false;
	bool down = false;
	bool up = false;
	bool space = false;
	void draw();
	public:
		void loop();
		Tetris(int, int);
		~Tetris();
};
#endif
