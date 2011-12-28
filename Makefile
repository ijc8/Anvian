CC=gcc
OPTS=-Wall
LIBS=-lglut -lGLEW -lGL -lSDL -lSDL_image
SOURCES=main.c util.c matrix.c
HEADERS=util.h

all: $(SOURCES) $(HEADERS)
	$(CC) $(OPTS) $(LIBS) -o main $(SOURCES)