#include <SDL2/SDL.h>
#include <vector>
#include <tuple>
#include <array>
#ifndef _TETRIS_H
#define _TETRIS_H
class Tetris {
	// coords are stored in X, Y format
	// which might be confusing as every matrix is accessed as Y, X or ROW, COLUMN
	//
	// Structure of Tetromino is:
	// 	RGB (first three elements of tuple)
	// 	last of element of tuple is top left coord of rotation matrix for tetromino
	// 	array has coords of tetromino coords
	typedef std::pair<std::tuple<int, int, int, std::pair<int, int>>,
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
	SDL_Renderer *renderer = nullptr;
	// I J L O S T Z
	std::array<Tetromino, 7> Tetrominos;
	std::array<double, 15> LevelSpeed; 
	std::array<int, 7> sequence;
	Tetromino currentTetromino;
	const int LINE_CLEAR_LEVELUP_AMOUNT = 10;
	const int blocksize = 25;
	const int TETRIS_PLAYFIELD_WIDTH = 10;
	const int TETRIS_PLAYFIELD_HEIGHT = 20;
	const int TETROMINO_SIZE = 4;
	const int MAX_LEVELS = 15;
	int sequence_index = 0;
	int xleftborder;
	int xrightborder;
	int level = 0;
	int lines_cleared = 0;
	const double ROTATE_SPEED = .1;
	const double SOFT_MOVE_SPEED = .1;
	const double LOCK_DELAY = .5;
	bool left = false;
	bool right = false;
	bool down = false;
	bool up = false;
	bool space = false;
	void draw();
	void hardDrop();
	void rotate();
	bool checkLeft();
	bool checkRight();
	bool checkDOWN();
	bool checkCollision();
	bool checkFill();
	void lock();
	void checkLineClear();
	void generateSequence();
	public:
		void loop();
		Tetris(int, int);
		~Tetris();
};
#endif
