objects = main.o Tetris.o
Tetris.run: $(objects)
	g++ -o Tetris.run $(objects) -lSDL2
main.o: main.cpp
	g++ -c main.cpp -Wall
Tetris.o: Tetris.cpp Tetris.h
	g++ -c Tetris.cpp -Wall
clean:
	rm Tetris $(objects)
