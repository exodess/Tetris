#ifndef SETTINGS_H
#define SETTINGS_H 

#include <ncurses.h>

/* тут хранятся основные константы, используемые в программе */

static const int count_points[5] = {0, 100, 300, 700, 1500};
enum {FAIL = 0, SUCCESS};
enum {TYPE_A = 0, TYPE_B, TYPE_C, TYPE_D, TYPE_E, TYPE_F, TYPE_G};
enum {LEFT, RIGHT};
enum {GAME, PAUSE};

typedef chtype pixel_t; 

/* структура фигуры */
typedef struct {
	char height;
	char width;
	pixel_t ** figure;
} shape_t;

/* обращение к пикселям поля происходит в виде: gfield[y][x] ! */
typedef pixel_t** field_t;

typedef struct {
	/* положение фигуры на поле */
	int x;
	int y;

	WINDOW * wgfield;
	WINDOW * wsidebar;
	WINDOW * wnextshape;
	
	field_t gfield;
	
} display_t;

typedef struct {
	int level;
	int record;
	int current_score;

	shape_t * current_shape;
	shape_t * next_shape;
	
} ginfo_t;

/* информация о кнопках управления */
#define KEY_PAUSE ' '
#define KEY_START_GAME KEY_PAUSE
#define KEY_TO_LEFT 'a'
#define KEY_TO_RIGHT 'd'
#define KEY_TO_DOWN 's'
#define KEY_ROTATE 'r'
#define KEY_SHOW_NEXT_FIGURE 'h'

/* настройки экрана */
#define SCREEN_WIDTH 10
#define SCREEN_HEIGHT 20
#define COUNT_SHAPES 7

/* для механики подсчета очков и уровней (доп 2,3) */
#define GET_POINTS_FOR_DESTROY_LINE(i) count_points[(i)]

#define COUNT_LEVELS 10
/* #define REQUIRED_COUNT_SCORES */
#define SAVE_FILE "bin/.save"

#define INITIAL_DELAY 200000
#define GET_HALFDELAY(level) (INITIAL_DELAY - ((level)*15000))
#define COUNT_POINTS_TO_LEVEL_UP 600

#endif
