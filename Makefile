CC = gcc
CFLAGS = -Wall -Wextra -Werror --std=c11
BIN_DIR = bin
TEST_DIR = tests
SRC_DIR = brickgame/tetris
GUI_DIR = gui/cli
SRC_FILES = $(SRC_DIR)/tetris.c $(SRC_DIR)/movement.c $(GUI_DIR)/display.c $(GUI_DIR)/display_managment.c
OBJ_FILES = $(SRC_FILES:.c=.o)

%.o : %.c
	$(CC) $(CFLAGS) -c $<

all : install

install : uninstall
	# sudo apt install libncursesw5-dev gcc
	mkdir $(BIN_DIR)
	$(CC) $(CFLAGS) $(SRC_FILES) -o $(BIN_DIR)/tetris -lncursesw
	./$(BIN_DIR)/tetris

uninstall : clean
	rm -rf $(BIN_DIR)
	
	
clean : 
	rm -rf *.o

	
