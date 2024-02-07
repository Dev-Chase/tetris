#gcc -framework CoreVideo -framework IOKit -framework Cocoa -framework GLUT -framework OpenGL lib/libraylib.a src/main.c -o bin/osx_build
CC = clang
LIB=lib
CFLAGS = -Iinclude -g -Wall -O2
BUILD_FLAGS=-framework CoreVideo -framework IOKit -framework Cocoa -framework GLUT -framework OpenGL $(LIB)/libraylib.a $(LIB)/dbl_linked_list.a
INCLUDE=include
SRC=src
OBJ=obj
OBJS=$(OBJ)/main.o $(OBJ)/game.o $(OBJ)/tetromino.o
BIN=bin
EXE=osx_build
MAIN=main


all: $(BIN)/$(EXE)

run: all
	$(BIN)/$(EXE)

# $< means first dependency
$(OBJ)/%.o: $(SRC)/%.c $(INCLUDE)/%.h 
	$(CC) $(CFLAGS) -c $< -o $@ 

$(OBJ)/main.o: $(SRC)/$(MAIN).c $(INCLUDE)/settings.h
	$(CC) $(CFLAGS) -c $< -o $@

$(BIN)/$(EXE): $(OBJS)
	$(CC) $(CFLAGS) $(BUILD_FLAGS) $^ -o $@

clean:
	rm -rf *.dSYM $(OBJ)/* $(BIN)/*
