all:
	g++ -Isrc/include -Lsrc/lib -o test main.cpp -lmingw32 -lSDL2main -lSDL2 -lSDL2_image