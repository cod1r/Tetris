#include <SDL2/SDL.h>
#include <vector>
#include <tuple>
#include <array>
#include <cstdint>
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
	typedef std::pair<std::tuple<int32_t, int32_t, int32_t, std::pair<int32_t, int32_t>>,
					std::array<std::pair<int32_t, int32_t>, 4>> Tetromino;
	enum TetrominoType {
		I,
		J,
		L,
		O,
		S,
		T,
		Z
	};
	static const int32_t TETRIS_PLAYFIELD_WIDTH = 10;
	static const int32_t TETRIS_PLAYFIELD_HEIGHT = 20;
	static const int32_t HEIGHT = 600;
	const int32_t WIDTH = 300;
	constexpr static int32_t blocksize = HEIGHT / TETRIS_PLAYFIELD_HEIGHT;
	int32_t playfield[TETRIS_PLAYFIELD_HEIGHT][TETRIS_PLAYFIELD_WIDTH] = {
		{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
		{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
		{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
		{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
		{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
		{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
		{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
		{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
		{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
		{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
		{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
		{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
		{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
		{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
		{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
		{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
		{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
		{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
		{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
		{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
	};
	SDL_Window *window = nullptr;
	SDL_Renderer *renderer = nullptr;
	// I J L O S T Z
	std::array<Tetromino, 7> Tetrominos;
	std::array<double, 15> LevelSpeed; 
	std::array<int32_t, 7> sequence;
	Tetromino currentTetromino;
	const int32_t LINE_CLEAR_LEVELUP_AMOUNT = 10;
	const int32_t TETROMINO_SIZE = 4;
	const int32_t MAX_LEVELS = 15;
	int32_t sequence_index = 0;
	int32_t xleftborder;
	int32_t xrightborder;
	int32_t level = 0;
	int32_t lines_cleared = 0;
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
	GLuint shader_program;
	public:
		void loop();
		Tetris();
		~Tetris();
};
#endif
