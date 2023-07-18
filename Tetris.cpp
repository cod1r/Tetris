#include <GL/glew.h>
#include <string>
#include <cstdint>
#include <iostream>
#include <fstream>
#include <vector>
#include <SDL2/SDL.h>
#include <stdexcept>
#include <chrono>
#include <climits>
#include <random>
#include <set>
#include "Tetris.h"

Tetris::Tetris()
{
	Tetris::Tetrominos[Tetris::I] = Tetris::Tetromino {
		{
			173,
			216,
			230,
			{3, 0}
		},
		std::array<std::pair<int, int>, 4>{
			{
				{3, 1},
				{4, 1},
				{5, 1},
				{6, 1},
			}
		}
	};
	Tetris::Tetrominos[Tetris::L] = Tetris::Tetromino {
		{
			0,
			0,
			139,
			{2, 0}
		},
		std::array<std::pair<int, int>, 4>{
			{
				{2, 1},
				{3, 1},
				{4, 1},
				{4, 0},
			}
		}
	};
	Tetris::Tetrominos[Tetris::J] = Tetris::Tetromino {
		{
			240,
			165,
			0,
			{2, 0}
		},
		std::array<std::pair<int, int>, 4>{
			{
				{2, 0},
				{2, 1},
				{3, 1},
				{4, 1},
			}
		}
	};
	// O doesn't need to be rotated
	Tetris::Tetrominos[Tetris::O] = Tetris::Tetromino {
		{
			255,
			255,
			0,
			{3, -1}
		},
		std::array<std::pair<int, int>, 4>{
			{
				{4, 0},
				{5, 0},
				{4, 1},
				{5, 1},
			}
		}
	};
	Tetris::Tetrominos[Tetris::T] = Tetris::Tetromino {
		{
			128,
			0,
			128,
			{2, 0}
		},
		std::array<std::pair<int, int>, 4>{
			{
				{2, 1},
				{3, 0},
				{3, 1},
				{4, 1},
			}
		}
	};
	Tetris::Tetrominos[Tetris::S] = Tetris::Tetromino {
		std::tuple<int, int, int, std::pair<int, int>>{
			0,
			255,
			0,
			{2, 0}
		},
		std::array<std::pair<int, int>, 4>{
			{
				{2, 1},
				{3, 0},
				{3, 1},
				{4, 0},
			}
		}
	};
	Tetris::Tetrominos[Tetris::Z] = Tetris::Tetromino {
		std::tuple<int, int, int, std::pair<int, int>>{
			255,
			0,
			0,
			{2, 0}
		},
		std::array<std::pair<int, int>, 4>{
			{
				{2, 0},
				{3, 0},
				{3, 1},
				{4, 1},
			}
		}
	};
	Tetris::LevelSpeed = std::array<double, 15>{
		1.0,
		0.79300,
		0.6178,
		0.47273,
		0.35520,
		0.26200,
		0.18968,
		0.13473,
		0.09388,
		0.06415,
		0.04298,
		0.02822,
		0.01815,
		0.01144,
		0.00706
	};
	int result = SDL_Init(SDL_INIT_VIDEO);
	if (result < 0) {
		throw std::runtime_error(SDL_GetError());
	}
	Tetris::window = SDL_CreateWindow(
		"Tetris",
		0,
		0,
		Tetris::WIDTH,
		Tetris::HEIGHT,
		SDL_WINDOW_OPENGL
	);
	SDL_GLContext ctx = SDL_GL_CreateContext(Tetris::window);
	if (ctx == NULL) {
		std::cerr << SDL_GetError() << std::endl;
		throw;
	}
	int res = SDL_GL_MakeCurrent(Tetris::window, ctx);
	if (res < 0) {
		std::cerr << SDL_GetError() << std::endl;
		throw;
	}
	GLenum glew_init_res = glewInit();
	if (glew_init_res != GLEW_OK) {
		std::cerr << "Unable to initialize GLEW" << std::endl;
		std::cerr << glewGetErrorString(glew_init_res) << std::endl;
		throw;
	}
	Tetris::shader_program = glCreateProgram();
	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	std::ifstream vertex;
	vertex.open("shaders/vertex_shader.glsl");
	if (vertex.is_open()) {
		std::string v_src;
		while (!vertex.eof()) {
			v_src += vertex.get();
		}
		vertex.close();
		int length = v_src.length();
		const GLchar* src = v_src.c_str();
		glShaderSource(vs, 1, &src, &length);
		glCompileShader(vs);
		int status;
		glGetShaderiv(vs, GL_COMPILE_STATUS, &status);
		if (!status) {
			std::cerr << "COMPILATION FAILED" << std::endl;
			int info_log_length;
			glGetShaderiv(vs, GL_INFO_LOG_LENGTH, &info_log_length);
			std::string info_log(info_log_length, ' ');
			glGetShaderInfoLog(vs, info_log.length(), &info_log_length, info_log.data());
			std::cerr << info_log << std::endl;
			throw;
		}
		glAttachShader(Tetris::shader_program, vs);
		glLinkProgram(Tetris::shader_program);

		GLuint VBO;
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		float vertices[] = {
			-0.05f, -0.05f,
			0.05f, -0.05f,
			0.0f,  0.05f
		};
		glBufferData(
				GL_ARRAY_BUFFER,
				sizeof(vertices),
				vertices,
				GL_DYNAMIC_DRAW
				);
		GLint location = glGetAttribLocation(Tetris::shader_program, "pos");
		if (location == -1) {
			std::cerr << "FUCK" << std::endl;
			throw;
		}
		glVertexAttribPointer(location, 2, GL_FLOAT, false, 2 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(location);

	} else {
		std::cerr << "Shader failed to open" << std::endl;
		throw;
	}
	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
	std::ifstream fragment;
	fragment.open("shaders/fragment_shader.glsl");
	if (fragment.is_open()) {
		std::string f_src;
		while (!fragment.eof()) {
			f_src += fragment.get();
		}
		fragment.close();
		int length = f_src.length();
		const GLchar* src = f_src.c_str();
		glShaderSource(fs, 1, &src, &length);
		glCompileShader(fs);
		int status;
		glGetShaderiv(fs, GL_COMPILE_STATUS, &status);
		if (!status) {
			std::cerr << "COMPILATION FAILED" << std::endl;
			int info_log_length;
			glGetShaderiv(fs, GL_INFO_LOG_LENGTH, &info_log_length);
			std::string info_log(info_log_length, ' ');
			glGetShaderInfoLog(fs, info_log.length(), &info_log_length, info_log.data());
			std::cerr << info_log << std::endl;
			throw;
		}
		glAttachShader(Tetris::shader_program, fs);
		glLinkProgram(Tetris::shader_program);
	} else {
		std::cerr << "Fragment Shader failed to open" << std::endl;
		throw;
	}


	//Tetris::renderer = SDL_CreateRenderer(window, -1, 0);
	Tetris::generateSequence();
	Tetris::currentTetromino = Tetris::Tetrominos[Tetris::sequence.at(Tetris::sequence_index)];
	Tetris::xleftborder = \
		(WIDTH - Tetris::blocksize * Tetris::TETRIS_PLAYFIELD_WIDTH) / 2;
	Tetris::xrightborder = \
		Tetris::xleftborder + \
		(Tetris::blocksize * Tetris::TETRIS_PLAYFIELD_WIDTH);
}

bool Tetris::checkFill()
{
	for (std::pair<int, int> p : Tetris::currentTetromino.second) {
		if (Tetris::playfield[p.second][p.first] != -1) {
			return false;
		}
	}
	return true;
}

void Tetris::rotate()
{
	Tetris::Tetromino tempT = Tetris::currentTetromino;
	if (Tetris::sequence.at(Tetris::sequence_index) == Tetris::I ||
			Tetris::sequence.at(Tetris::sequence_index) == Tetris::O) {
		const int rotateMatrixSize = 4;
		for (int t = 0; t < Tetris::TETROMINO_SIZE; ++t) {
			tempT.second.at(t).first -= std::get<3>(tempT.first).first;
			tempT.second.at(t).second -= std::get<3>(tempT.first).second;

			int old_c = tempT.second.at(t).first;
			tempT.second.at(t).first = rotateMatrixSize - 1 - tempT.second.at(t).second;
			tempT.second.at(t).second = old_c;

			tempT.second.at(t).first += std::get<3>(tempT.first).first;
			tempT.second.at(t).second += std::get<3>(tempT.first).second;
		}
	} else {
		const int rotateMatrixSize = 3;
		for (int t = 0; t < Tetris::TETROMINO_SIZE; ++t) {
			tempT.second.at(t).first -= std::get<3>(tempT.first).first;
			tempT.second.at(t).second -= std::get<3>(tempT.first).second;

			int old_c = tempT.second.at(t).first;
			tempT.second.at(t).first = rotateMatrixSize - 1 - tempT.second.at(t).second;
			tempT.second.at(t).second = old_c;

			tempT.second.at(t).first += std::get<3>(tempT.first).first;
			tempT.second.at(t).second += std::get<3>(tempT.first).second;
		}
	}
	bool swappable = true;
	int miny = INT_MAX;
	int maxy = INT_MIN;
	int minx = INT_MAX;
	int maxx = INT_MIN;
	for (int t = 0; t < Tetris::TETROMINO_SIZE; ++t) {
		miny = SDL_min(miny, tempT.second.at(t).second);
		maxy = SDL_max(maxy, tempT.second.at(t).second);
		minx = SDL_min(minx, tempT.second.at(t).first);
		maxx = SDL_max(maxx, tempT.second.at(t).first);
	}
	if (miny < 0) {
		for (int t = 0; t < Tetris::TETROMINO_SIZE; ++t) {
			tempT.second.at(t).second += SDL_abs(miny);
		}
	} else if (maxy >= Tetris::TETRIS_PLAYFIELD_HEIGHT) {
		for (int t = 0; t < Tetris::TETROMINO_SIZE; ++t) {
			tempT.second.at(t).second -= SDL_abs(maxy - (Tetris::TETRIS_PLAYFIELD_HEIGHT - 1));
		}
	}

	if (minx < 0) {
		for (int t = 0; t < Tetris::TETROMINO_SIZE; ++t) {
			tempT.second.at(t).first += SDL_abs(minx);
		}
	} else if (maxx >= Tetris::TETRIS_PLAYFIELD_WIDTH) {
		for (int t = 0; t < Tetris::TETROMINO_SIZE; ++t) {
			tempT.second.at(t).first -= SDL_abs(maxx - (Tetris::TETRIS_PLAYFIELD_WIDTH - 1));
		}
	}
	for (std::pair<int, int> p : tempT.second) {
		if (p.first < 0 ||
				p.first >= Tetris::TETRIS_PLAYFIELD_WIDTH ||
				p.second < 0 ||
				p.second >= Tetris::TETRIS_PLAYFIELD_HEIGHT ||
				Tetris::playfield[p.second][p.first] != -1
		) {
			swappable = false;
		}
	}
	if (swappable) {
		Tetris::currentTetromino = tempT;
	}
}

void Tetris::hardDrop()
{
	int min_add = INT_MAX;
	for (int t = 0; t < Tetris::TETROMINO_SIZE; ++t) {
		min_add = SDL_min(min_add, Tetris::TETRIS_PLAYFIELD_HEIGHT - 1 - Tetris::currentTetromino.second.at(t).second);
		for (int Y = Tetris::TETRIS_PLAYFIELD_HEIGHT - 1; Y > Tetris::currentTetromino.second.at(t).second; --Y) {
			if (Tetris::playfield[Y][Tetris::currentTetromino.second.at(t).first] != -1) {
				min_add = SDL_min(min_add, Y - 1 - Tetris::currentTetromino.second.at(t).second);
			}
		}
	}
	std::get<3>(Tetris::currentTetromino.first).second += min_add;
	for (int t = 0; t < Tetris::TETROMINO_SIZE; ++t) {
		Tetris::currentTetromino.second.at(t).second += min_add;
	}
}

void Tetris::lock()
{
	for (int i = 0; i < Tetris::TETROMINO_SIZE; ++i) {
		Tetris::playfield[Tetris::currentTetromino.second.at(i).second][Tetris::currentTetromino.second.at(i).first] = \
			Tetris::sequence.at(Tetris::sequence_index);
	}
	++Tetris::sequence_index;
	if (Tetris::sequence_index == 7) {
		Tetris::generateSequence();
		Tetris::sequence_index = 0;
	}
	Tetris::currentTetromino = Tetris::Tetrominos.at(Tetris::sequence.at(Tetris::sequence_index));
}

bool Tetris::checkCollision()
{
	for (int i = 0; i < Tetris::TETROMINO_SIZE; ++i) {
		if (Tetris::currentTetromino.second.at(i).second + 1 == Tetris::TETRIS_PLAYFIELD_HEIGHT ||
				(Tetris::currentTetromino.second.at(i).second + 1 < Tetris::TETRIS_PLAYFIELD_HEIGHT &&
				Tetris::playfield[Tetris::currentTetromino.second.at(i).second + 1][Tetris::currentTetromino.second.at(i).first] != -1)
		) {
				return true;
		}
	}
	return false;
}

void Tetris::checkLineClear()
{
	for (int Y = 0; Y < Tetris::TETRIS_PLAYFIELD_HEIGHT; ++Y) {
		int counter = 0;
		for (int X = 0; X < Tetris::TETRIS_PLAYFIELD_WIDTH; ++X) {
			if (Tetris::playfield[Y][X] != -1) {
				++counter;
			}
		}
		if (counter == Tetris::TETRIS_PLAYFIELD_WIDTH) {
			++Tetris::lines_cleared;
			for (int X2 = 0; X2 < Tetris::TETRIS_PLAYFIELD_WIDTH; ++X2) {
				Tetris::playfield[Y][X2] = -1;
			}
			for (int Y2 = Y; Y2 > 0; --Y2) {
				for (int X2 = 0; X2 < Tetris::TETRIS_PLAYFIELD_WIDTH; ++X2) {
					std::swap(Tetris::playfield[Y2][X2], Tetris::playfield[Y2 - 1][X2]);
				}
			}
		}
	}
	if (Tetris::lines_cleared == Tetris::LINE_CLEAR_LEVELUP_AMOUNT) {
		++Tetris::level;
		Tetris::lines_cleared = 0;
	}
}

void Tetris::generateSequence()
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> distrib(0, 6);
	std::set<int> seen;
	int temp_sequence_index = 0;
	while (seen.size() < 7) {
		int a = distrib(gen);
		if (!seen.contains(a)) {
			seen.insert(a);
			Tetris::sequence.at(temp_sequence_index++) = a;
		}
	}
}

bool Tetris::checkLeft()
{
	for (int i = 0; i < Tetris::TETROMINO_SIZE; ++i) {
		if (Tetris::currentTetromino.second.at(i).first - 1 < 0 ||
				(Tetris::currentTetromino.second.at(i).first - 1 >= 0 &&
				 Tetris::playfield[Tetris::currentTetromino.second.at(i).second][Tetris::currentTetromino.second.at(i).first - 1] != -1))
			return false;
	}
	return true;
}

bool Tetris::checkRight()
{
	for (int i = 0; i < Tetris::TETROMINO_SIZE; ++i) {
		if (Tetris::currentTetromino.second.at(i).first + 1 == Tetris::TETRIS_PLAYFIELD_WIDTH ||
				(Tetris::currentTetromino.second.at(i).first + 1 < Tetris::TETRIS_PLAYFIELD_WIDTH &&
				 Tetris::playfield[Tetris::currentTetromino.second.at(i).second][Tetris::currentTetromino.second.at(i).first + 1] != -1))
			return false;
	}
	return true;
}

bool Tetris::checkDOWN()
{
	for (int i = 0; i < Tetris::TETROMINO_SIZE; ++i) {
		if (Tetris::currentTetromino.second.at(i).second + 1 == Tetris::TETRIS_PLAYFIELD_HEIGHT ||
				(Tetris::currentTetromino.second.at(i).second + 1 < Tetris::TETRIS_PLAYFIELD_HEIGHT &&
				 Tetris::playfield[Tetris::currentTetromino.second.at(i).second + 1][Tetris::currentTetromino.second.at(i).first] != -1)) {
			return false;
		}
	}
	return true;
}

void Tetris::loop()
{
	SDL_Event e;
	std::chrono::time_point<std::chrono::system_clock> prev = std::chrono::system_clock::now();
	std::chrono::time_point<std::chrono::system_clock> lastInputTimeDown = std::chrono::system_clock::now();
	std::chrono::time_point<std::chrono::system_clock> lastInputTimeLeft = std::chrono::system_clock::now();
	std::chrono::time_point<std::chrono::system_clock> lastInputTimeRight = std::chrono::system_clock::now();
	std::chrono::time_point<std::chrono::system_clock> lastInputTimeUp = std::chrono::system_clock::now();
	std::chrono::time_point<std::chrono::system_clock> collideTime;
	bool collided = false;
	while (Tetris::level < Tetris::MAX_LEVELS && Tetris::checkFill()) {
		while (SDL_PollEvent(&e)) {
			switch (e.type) {
				case SDL_KEYDOWN:
					if (e.key.keysym.sym == SDLK_q) {
						return;
					} else if (e.key.keysym.sym == SDLK_SPACE) {
						Tetris::space = true;
					} else if (e.key.keysym.sym == SDLK_UP) {
						Tetris::up = true;
					} else if (e.key.keysym.sym == SDLK_RIGHT) {
						Tetris::right = true;
					} else if (e.key.keysym.sym == SDLK_LEFT) {
						Tetris::left = true;
					} else if (e.key.keysym.sym == SDLK_DOWN) {
						Tetris::down = true;
					}
					break;
				case SDL_KEYUP:
					if (e.key.keysym.sym == SDLK_RIGHT) {
						Tetris::right = false;
					} else if (e.key.keysym.sym == SDLK_LEFT) {
						Tetris::left = false;
					} else if (e.key.keysym.sym == SDLK_DOWN) {
						Tetris::down = false;
					} else if (e.key.keysym.sym == SDLK_UP) {
						Tetris::up = false;
					} else if (e.key.keysym.sym == SDLK_SPACE) {
						Tetris::space = false;
					}
					break;
				default:
					break;
			}
		}
		if (Tetris::up &&
				std::chrono::duration<double>(std::chrono::system_clock::now() - \
					lastInputTimeUp).count() >= Tetris::ROTATE_SPEED
		) {
			lastInputTimeUp = std::chrono::system_clock::now();
			Tetris::rotate();
		}
		if (
				Tetris::left &&
				std::chrono::duration<double>(std::chrono::system_clock::now() - \
					lastInputTimeLeft).count() >= Tetris::SOFT_MOVE_SPEED &&
				Tetris::checkLeft()
		) {
			lastInputTimeLeft = std::chrono::system_clock::now();
			--std::get<3>(Tetris::currentTetromino.first).first;
			for (int i = 0; i < Tetris::TETROMINO_SIZE; ++i) {
				--Tetris::currentTetromino.second.at(i).first;
			}
		}
		if (
				Tetris::right &&
				std::chrono::duration<double>(std::chrono::system_clock::now() - \
					lastInputTimeRight).count() >= Tetris::SOFT_MOVE_SPEED &&
				Tetris::checkRight()
		) {
			lastInputTimeRight = std::chrono::system_clock::now();
			++std::get<3>(Tetris::currentTetromino.first).first;
			for (int i = 0; i < Tetris::TETROMINO_SIZE; ++i) {
				++Tetris::currentTetromino.second.at(i).first;
			}
		}
		if (
				Tetris::down &&
				std::chrono::duration<double>(std::chrono::system_clock::now() - \
					lastInputTimeDown).count() >= Tetris::SOFT_MOVE_SPEED &&
				Tetris::checkDOWN()
		) {
			lastInputTimeDown = std::chrono::system_clock::now();
			++std::get<3>(Tetris::currentTetromino.first).second;
			for (int i = 0; i < Tetris::TETROMINO_SIZE; ++i) {
				++Tetris::currentTetromino.second.at(i).second;
			}
		}

		if (Tetris::space) {
			Tetris::hardDrop();
		}

		std::chrono::time_point<std::chrono::system_clock> curr = std::chrono::system_clock::now();
		bool right_time = \
			std::chrono::duration<double>(curr - prev).count() >= Tetris::LevelSpeed.at(Tetris::level);

		if (right_time && !Tetris::down && Tetris::checkDOWN()) {
			prev = std::chrono::system_clock::now();
			++std::get<3>(Tetris::currentTetromino.first).second;
			for (int i = 0; i < Tetris::TETROMINO_SIZE; ++i) {
				++Tetris::currentTetromino.second.at(i).second;
			}
		}

		if (Tetris::checkCollision()) {
			if (!collided) {
				collideTime = std::chrono::system_clock::now();
			}
			collided = true;
		} else {
			collided = false;
		}
		if (collided &&
				std::chrono::duration<double>(std::chrono::system_clock::now() - collideTime).count() >= Tetris::LOCK_DELAY) {
			collideTime = std::chrono::system_clock::now();
			Tetris::lock();
			collided = false;
		}
		Tetris::checkLineClear();
		Tetris::draw();
	}
}

void Tetris::draw()
{
	GLint out_color_location = glGetUniformLocation(this->shader_program, "out_color");
	if (out_color_location == -1) {
		std::cerr << "FUCK" << std::endl;
		throw;
	}
	glUniform4f(out_color_location, 0.0f, 1.0f, 0.5f, 1.0f);
	glUseProgram(this->shader_program);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	SDL_GL_SwapWindow(this->window);

	// -- old
	//SDL_SetRenderDrawColor(Tetris::renderer, 0, 0, 0, 255);
	//SDL_RenderClear(Tetris::renderer);
	//SDL_SetRenderDrawColor(Tetris::renderer, 100, 100, 100, 255);
	//SDL_Rect playfield_area {
	//	Tetris::xleftborder,
	//	0,
	//	Tetris::blocksize * Tetris::TETRIS_PLAYFIELD_WIDTH,
	//	Tetris::blocksize * Tetris::TETRIS_PLAYFIELD_HEIGHT
	//};
	//SDL_RenderFillRect(Tetris::renderer, &playfield_area);
	//SDL_SetRenderDrawColor(Tetris::renderer, 255, 255, 255, 255);
	//for (int Y = 0; Y < Tetris::TETRIS_PLAYFIELD_HEIGHT; ++Y) {
	//	for (int X = 0; X < Tetris::TETRIS_PLAYFIELD_WIDTH; ++X) {
	//		if (Tetris::playfield[Y][X] != -1) {
	//			SDL_SetRenderDrawColor(
	//				Tetris::renderer,
	//				std::get<0>(Tetris::Tetrominos.at(Tetris::playfield[Y][X]).first),
	//				std::get<1>(Tetris::Tetrominos.at(Tetris::playfield[Y][X]).first),
	//				std::get<2>(Tetris::Tetrominos.at(Tetris::playfield[Y][X]).first),
	//				255
	//			);
	//			SDL_Rect temp {
	//				Tetris::xleftborder + Tetris::blocksize * X,
	//				Tetris::blocksize * Y,
	//				Tetris::blocksize,
	//				Tetris::blocksize
	//			};
	//			SDL_RenderFillRect(Tetris::renderer, &temp);
	//		}
	//	}
	//}
	//for (int i = 0; i < Tetris::TETROMINO_SIZE; ++i) {
	//	SDL_SetRenderDrawColor(
	//		Tetris::renderer,
	//		std::get<0>(Tetris::currentTetromino.first),
	//		std::get<1>(Tetris::currentTetromino.first),
	//		std::get<2>(Tetris::currentTetromino.first),
	//		255
	//	);
	//	SDL_Rect temp {
	//		Tetris::xleftborder + Tetris::blocksize * Tetris::currentTetromino.second.at(i).first,
	//		Tetris::blocksize * Tetris::currentTetromino.second.at(i).second,
	//		Tetris::blocksize,
	//		Tetris::blocksize
	//	};
	//	SDL_RenderFillRect(Tetris::renderer, &temp);
	//}
	//SDL_RenderPresent(Tetris::renderer);
}

Tetris::~Tetris()
{
	SDL_DestroyRenderer(Tetris::renderer);
	SDL_DestroyWindow(Tetris::window);
	SDL_Quit();
}
