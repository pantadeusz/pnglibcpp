all: tests

pnglib.o: pnglib.cpp
	g++ -ggdb -I./include pnglib.cpp -c

unit_pnglib.o: unit_pnglib.cpp
	g++ -ggdb -I./include -I./thirdparty unit_pnglib.cpp -c

unit_pnglib:unit_pnglib.o pnglib.o
	g++ -ggdb unit_pnglib.o pnglib.o -lpng -o unit_pnglib


tests:unit_pnglib
	./unit_pnglib

clean:
	rm -f unit_pnglib *.o data/b.png

