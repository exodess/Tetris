# Tetris

The project is a terminal implementation of the Tetris game

Developed in C using the ncurses library

## Installation

- For the game to work properly, you need to install additional tools
  (Ubuntu/Debian)
```bash
  sudo apt install libncursesw5-dev gcc git
```

- After installing the necessary tools, you need to clone this repository
```bash
git clone git@github.com:exodess/Tetris.git
```

- Installing the game
```bash
cd Tetris && make install
```

- Start the game
```bash
make start
```

## Controls
- 'a' - movement of the figure to the left
- 'd' - movement of the figure to the right
- 's' - accelerating the fall of the figure
- 'r' - figure rotation
- 'h' - showing the next shape
