CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c11 -D_XOPEN_SOURCE=600 -D_POSIX_C_SOURCE=200112L
BIN_DIR = bin
TEST_DIR = tests
SRC_DIR = brickgame/tetris
GUI_DIR = gui/cli
SRC_FILES = $(SRC_DIR)/tetris.c $(SRC_DIR)/movement.c $(GUI_DIR)/display.c $(GUI_DIR)/memory_managment.c
OBJ_FILES = $(SRC_FILES:.c=.o)

%.o : %.c
	$(CC) $(CFLAGS) -c $<

all : install

install : uninstall
	mkdir $(BIN_DIR)
	$(CC) $(CFLAGS) $(SRC_FILES) -o $(BIN_DIR)/tetris -lncursesw
	./$(BIN_DIR)/tetris

uninstall : clean
	rm -rf $(BIN_DIR)

start:
	./$(BIN_DIR)/tetris
	
clean : 
	rm -rf *.o

	
