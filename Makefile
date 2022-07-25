objects = main.o Tetris.o
Tetris.run: $(objects)
	g++ -o Tetris.run $(objects) -lSDL2
main.o: main.cpp
	g++ -c main.cpp -Wall -std=c++20
Tetris.o: Tetris.cpp Tetris.h
	g++ -c Tetris.cpp -Wall -std=c++20
clean:
	rm Tetris $(objects)
