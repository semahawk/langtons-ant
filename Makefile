CC = clang
CFLAGS = -std=c99 `sdl-config --cflags`
LIBS = `sdl-config --libs`

.PHONY: all clean distclean

all: ant

ant: ant.c
	$(CC) $(CFLAGS) $< -o $@ $(LIBS)

clean:
	rm -rf *.o

distclean: clean
	rm -rf ant

