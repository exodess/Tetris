#include <stdio.h>
#include <string.h>
#include "../../common/settings.h"
#include "drowing.h"

void display(display_t * display_info, ginfo_t * game_info)
{
	/* Основная функция, отвечающая за прорисовку игры 

		Вызывает две вспомогательные функции:
		1. drowing_game_field() - отвечает за прорисовку окна игрового поля
		2. drowing_sidebar() - отвечает за прорисовку бокового окна с игровой информацией

		Для каждой функции было инициализировано окно, 
			где она будет рисовать необходимые фигуры 
		
	*/
	/* wclear(display_info->wgfield); */
	/* wclear(display_info->wsidebar); */

	clear();

	drowing_game_field(display_info, game_info);
	drowing_sidebar(display_info, game_info);
	
}

void drowing_game_field(display_t * display_info, ginfo_t * game_info)
{
	WINDOW * win = display_info->wgfield;
	field_t gfield = display_info->gfield;

	box(win, 0, 0);

	for(int i = 0; i < SCREEN_HEIGHT; i++)
		for(int j = 0; j < SCREEN_WIDTH; j++)
			show_pixel(win, i + 1, j + 1, gfield[i][j]);

	for(int i = 0; i < game_info->current_shape->height; i++)
		for(int j = 0; j < game_info->current_shape->width; j++)
			show_pixel(win, display_info->y + i + 1, display_info->x + j + 1, game_info->current_shape->figure[i][j]);

	wrefresh(win);
	
}

void drowing_start_place()
{
	/* Окно приветствия пользователя */
	box(stdscr, 0, 0); 
	mvwprintw(stdscr, SCREEN_HEIGHT / 2 - 1, 5, "Welcome to the TETRIS");
	mvwprintw(stdscr, SCREEN_HEIGHT / 2 + 1, 5, "Press any key to start the game..."); 

	wrefresh(stdscr);            
}

void drowing_pause_place(display_t * display_info)
{
	WINDOW * win = display_info->wsidebar;
	char * mess_pause = "PAUSE";

	for(size_t i = 0; i < strlen(mess_pause); i++)
		mvwaddch(win, 2, 7+i, mess_pause[i] | A_BLINK);

	wrefresh(win);
	
}

void drowing_sidebar(display_t * display_info, ginfo_t * game_info)
{
	char * message_level =  "Level : %d";
	char * message_scores = "Score : %d";
	char * message_record = "Record: %d";

	WINDOW * win = display_info->wsidebar;
	wclear(win);
	box(win, 0, 0);

	#if SCREEN_WIDTH == 10
		mvwprintw(win, 1, 3, "TETRIS");
		mvwprintw(win, 3, 1, message_level, game_info->level);
		mvwprintw(win, 6, 1, message_scores, game_info->current_score);
		mvwprintw(win, 10, 1, message_record, game_info->record);
	#else
		char * message_tetris = "TETRIS";
		mvwprintw(win, 1, (SCREEN_WIDTH - strlen(message_tetris)) / 2, "TETRIS");
		mvwprintw(win, 3, (SCREEN_WIDTH - strlen(message_level)) / 2, message_level, game_info->level);
		mvwprintw(win, 6, (SCREEN_WIDTH - strlen(message_scores)) / 2, message_scores, game_info->current_score);
		mvwprintw(win, 10, (SCREEN_WIDTH - strlen(message_record)) / 2, message_record, game_info->record);
	#endif
	
	wrefresh(win);
}

void show_pixel(WINDOW * win, int y, int x, pixel_t pxl) 
{ 
	if(GET_VALUE(pxl))mvwaddch(win, y, x, ACS_DIAMOND); 
} 
