all:

pnglib.o: pnglib.cpp
	g++ -ggdb -I. -std=c++0x pnglib.cpp -c

unit_pnglib.o: unit_pnglib.cpp
	g++ -ggdb -I. -I./thirdparty -std=c++0x unit_pnglib.cpp -c

unit_pnglib:unit_pnglib.o pnglib.o
	g++ -ggdb unit_pnglib.o pnglib.o -lpng -o unit_pnglib


tests:unit_pnglib
	./unit_pnglib


