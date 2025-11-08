output: main.cpp logic.cpp
	g++ main.cpp logic.cpp -g -o main.exe -I./SDL2 -L./lib -lmingw32 -lSDL2main -lSDL2