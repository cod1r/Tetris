objects = main.o Tetris.o
Tetris.run: $(objects)
	g++ -o Tetris.run $(objects) `sdl2-config --cflags --libs`
main.o: main.cpp
	g++ -c main.cpp -Wall -std=c++20 `sdl2-config --cflags --libs`
Tetris.o: Tetris.cpp Tetris.h
	g++ -c Tetris.cpp -Wall -std=c++20 `sdl2-config --cflags --libs`
clean:
	rm Tetris.run $(objects)
