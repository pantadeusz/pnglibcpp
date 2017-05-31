all:
	g++ -ggdb -I. -std=c++0x pnglib.cpp -c
	g++ -ggdb -I. -DUNITTEST -std=c++0x pnglib.cpp -o pnglib -lpng


