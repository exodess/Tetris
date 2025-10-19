#include "drowing.h"
#include  "../../common/settings.h"

#include <stdlib.h> 

void init_game_field(display_t * display_info)
{
	/* 
		создается матрица размером 20*10, которая будет хранить в себе
		информацию о каждом пикселе 

		Параллельно с этим происходит инициализация ncurses для последующей работы 
	*/
	WINDOW * wgfield, * wsidebar;
	
	initscr(); /* начальная инициализация ncurses */
	noecho(); /* для того, чтобы вводимые символы не были видны */
	curs_set(0); /* делаем курсор невидимым */
	timeout(1); /* время ожидания для getch() */
	
	start_color();
	/* инициализируем цветовые схемы для вывода фигур */
	init_pair(1, COLOR_RED, COLOR_RED); 
	init_pair(2, COLOR_YELLOW, COLOR_YELLOW); 
	init_pair(3, COLOR_YELLOW, COLOR_YELLOW); 
	init_pair(4, COLOR_CYAN, COLOR_CYAN); 
	init_pair(5, COLOR_GREEN, COLOR_GREEN); 
	init_pair(6, COLOR_BLUE, COLOR_BLUE); 
	init_pair(7, COLOR_GREEN, COLOR_GREEN); 
	
	
	/* создаем подокна для отображения игрового поля и боковой панели */
	wgfield = subwin(stdscr, SCREEN_HEIGHT+2, SCREEN_WIDTH+2, 0, 0);
	wsidebar = subwin(stdscr, SCREEN_HEIGHT+2, SCREEN_WIDTH+3, 0, SCREEN_WIDTH + 1);
	box(wgfield, 0, 0);
	box(wsidebar, 0, 0);
	
	/* выделяем место для игрового поля */
	display_info->gfield = calloc(SCREEN_HEIGHT, sizeof(pixel_t *));
	
	for(int i = 0; i < SCREEN_HEIGHT; i++)
		display_info->gfield[i] = calloc(SCREEN_WIDTH, sizeof(pixel_t));


	display_info->wgfield = wgfield;
	display_info->wsidebar = wsidebar;
	display_info->wnextshape = NULL; /* По умолчанию окно для показа следующей
	                                    фигуры отсутствует */

}

void switch_next_shape_window(display_t * display_info)
{
	WINDOW * win = display_info->wnextshape;

	/* если окно было выключено, создаем его */
	if(win == NULL) 
		display_info->wnextshape = subwin(stdscr, 4, 6, SCREEN_HEIGHT - 6, SCREEN_WIDTH + 4);	

	/* если окно существовало, то удаляем его */
	else {
		delwin(win);

		display_info->wnextshape = NULL;
	}

}

void destroy_game_field(display_t * display_info)
{
	/*
		Освобождаем память 
		Также завершаем работу с ncurses
	*/
	
	for(int i = 0; i < SCREEN_HEIGHT; i++) {
		free(display_info->gfield[i]);
		display_info->gfield[i] = NULL;
	}

	free(display_info->gfield);
	display_info->gfield = NULL;

	delwin(display_info->wgfield);
	delwin(display_info->wsidebar);
	if(display_info->wnextshape) delwin(display_info->wnextshape);

	endwin();
		
}

void destroy_shape(shape_t * shape)
{
	if(shape && shape->figure) {
		for(int i = 0; i < shape->height; i++) {
			free(shape->figure[i]); 
			shape->figure[i] = NULL;
		}	
		
		free(shape->figure);
		shape->figure = NULL;	

	}
}

void init_shape(shape_t * shape)
{
	/* выделение памяти под фигуру 
			поля height и width должны быть определены заранее
	*/

		shape->figure = malloc(shape->height * sizeof(pixel_t *));
		
		for(int i = 0; i < shape->height; i++) {
			shape->figure[i] = malloc(shape->width * sizeof(pixel_t));
		}	
	
}

void create_shape(shape_t * shape, int type)
{
	/* главная функция для создания новой фиг */
	chtype nothing = '0';
	static const int heights_shapes[COUNT_SHAPES] = {1, 2, 2, 2, 2, 2, 2};
	static const int width_shapes[COUNT_SHAPES]   = {4, 3, 3, 2, 3, 3, 3};
	shape->height = heights_shapes[type%COUNT_SHAPES];
	shape->width = width_shapes[type % COUNT_SHAPES];
	
	init_shape(shape);

	for(int i = 0; i < shape->height; i++) 
		for(int j = 0; j < shape->width; j++) 
			shape->figure[i][j] = '1' + type%COUNT_SHAPES; 
			

	/* 
		матрица по умолчанию заполнена 1, 
		необходимо убрать лишние пиксели в фигурах:
	*/
	
	if(type == TYPE_B) {
		/* 
				*
				***
		*/
		shape->figure[0][1] = nothing;
		shape->figure[0][2] = nothing;
	}

	else if(type == TYPE_C) {
		/* 
				  *
				***
		*/

		shape->figure[0][0] = nothing;
		shape->figure[0][1] = nothing;
	}

	else if(type == TYPE_E) {
		/* 
				 **
				**
		*/
		shape->figure[0][0] = nothing;
		shape->figure[1][2] = nothing;
	}

	else if(type == TYPE_F) {
		/* 
				 *
				***
		*/

		shape->figure[0][0] = nothing;
		shape->figure[0][2] = nothing;
	}

	else if(type == TYPE_G) {
		/* 
				**
				 **
		*/

		shape->figure[0][2] = nothing;
		shape->figure[1][0] = nothing;
	}
}
