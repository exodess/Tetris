#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "../../common/settings.h"
#include "drowing.h"

void display(display_t * display_info, ginfo_t * game_info)
{
	/* Основная функция, отвечающая за прорисовку игры 

		Вызывает две вспомогательные функции:
		1. drowing_game_field() - отвечает за прорисовку окна игрового поля
		2. drowing_sidebar() - отвечает за прорисовку бокового окна с игровой информацией
		3. drowing_panel_next_shape() - отвечает за прорисовку окна,
			показывающего следующую фигуру
 
		Для каждой функции было инициализировано окно, 
			где она будет рисовать необходимые фигуры 
		
	*/

	clear(); 

	drowing_game_field(display_info, game_info);
	drowing_sidebar(display_info, game_info);
	drowing_panel_with_next_shape(display_info, game_info);
	
}

void drowing_game_field(display_t * display_info, ginfo_t * game_info)
{
	WINDOW * win = display_info->wgfield;
	field_t gfield = display_info->gfield;

	wclear(win);
	box(win, 0, 0); 

	for(int i = 0; i < SCREEN_HEIGHT; i++)
		for(int j = 0; j < SCREEN_WIDTH; j++) {
			wattron(win, COLOR_PAIR(gfield[i][j] - '0')); 
			show_pixel(win, i, j, gfield[i][j]);
			wattroff(win, COLOR_PAIR(gfield[i][j] - '0')); 
		}

	for(int i = 0; i < game_info->current_shape->height; i++)
		for(int j = 0; j < game_info->current_shape->width; j++) {
			wattron(win, COLOR_PAIR(game_info->current_shape->figure[i][j] - '0'));
			show_pixel(win, display_info->y + i, display_info->x + j, game_info->current_shape->figure[i][j]);
			wattroff(win, COLOR_PAIR(game_info->current_shape->figure[i][j] - '0'));
		}
	
	wrefresh(win);
	
}

void drowing_panel_with_next_shape(display_t * display_info, ginfo_t * game_info)
{
	WINDOW * win = display_info->wnextshape;

	if(win) { 
		box(win, 0, 0);
		
		for(int i = 0; i < game_info->next_shape->height; i++)
			for(int j = 0; j < game_info->next_shape->width; j++) {
				wattron(win, COLOR_PAIR(game_info->next_shape->figure[i][j] - '0'));
				show_pixel(win, i, j, game_info->next_shape->figure[i][j]);
				wattroff(win, COLOR_PAIR(game_info->next_shape->figure[i][j] - '0'));	
			}

		wrefresh(win);
	} 

}

void drowing_start_place(void)
{
	/* Окно приветствия пользователя */

	clear();
	
	mvwprintw(stdscr, SCREEN_HEIGHT / 2 - 6, 6, "Welcome to the TETRIS");
	mvwprintw(stdscr, SCREEN_HEIGHT / 2 - 3, 3, "Wait for the start of game..."); 

	/* обратный отчет до начала игры */
	for(int i = 3; i > 0; i--) {
		mvaddch(SCREEN_HEIGHT / 2, 15, '0' + i);
		wrefresh(stdscr);
		sleep(1);
	}    
	clear();
	wrefresh(stdscr);
	        
}

int drowing_exit_place(ginfo_t * game_info)
{
	/* окно выхода показывает итоговый результат и позволяет игроку выбрать - 
		играть снова или выйти из игры 
	*/

	clear();
	int choice = 0;
	chtype ch = KEY_TO_LEFT ;

	init_pair(10, COLOR_WHITE, COLOR_BLUE);

	mvwprintw(stdscr, SCREEN_HEIGHT / 2 - 6, SCREEN_WIDTH, "GAME OVER!");
	mvwprintw(stdscr, SCREEN_HEIGHT / 2 - 3, SCREEN_WIDTH, "Level: %d", game_info->level);
	mvwprintw(stdscr, SCREEN_HEIGHT / 2 - 1, SCREEN_WIDTH, "Score: %d", game_info->current_score);

	mvwprintw(stdscr, SCREEN_HEIGHT / 2 + 1, 1, "Do you want to continue? (ENTER)");

	do {
		if(ch == KEY_TO_LEFT) choice = 0;
		else if(ch == KEY_TO_RIGHT) choice = 1;

		wattron(stdscr, COLOR_PAIR(10));
		if(choice == 0) {
			/* выбрана левая кнопка "play" */
			mvprintw(SCREEN_HEIGHT / 2 + 3, 6, "play");
			wattroff(stdscr, COLOR_PAIR(10));
			mvprintw(SCREEN_HEIGHT / 2 + 3, 18, "exit");
			
		}

		else {
			/* выбрана правая кнопка "exit" */
			mvprintw(SCREEN_HEIGHT / 2 + 3, 18, "exit");
			wattroff(stdscr, COLOR_PAIR(10));
			mvprintw(SCREEN_HEIGHT / 2 + 3, 6, "play");
		}
	}
	while((ch = getch()) != '\n') ;

	return choice;
	
}



void drowing_pause_place(display_t * display_info, int is_clear)
{
	WINDOW * win = display_info->wsidebar;
	char * mess_pause = "PAUSE";
	chtype cur_ch = ' ';

	for(size_t i = 0; i < strlen(mess_pause); i++) {
		cur_ch = (is_clear) ? ' ' : mess_pause[i] | A_BLINK;
		mvwaddch(win, 2, 3+i, cur_ch);
	}

	wrefresh(win);
}

void drowing_sidebar(display_t * display_info, ginfo_t * game_info)
{
	WINDOW * win = display_info->wsidebar;
	wclear(win);
	box(win, 0, 0); 

	mvwprintw(win, 1, 3, "TETRIS");
	mvwprintw(win, 3, 1, "Level");
	mvwprintw(win, 4, 2, "%d", game_info->level);
	mvwprintw(win, 6, 1, "Score");
	mvwprintw(win, 7, 2, "%d", game_info->current_score);
	mvwprintw(win, 9, 1, "Record");
	mvwprintw(win, 10, 2, "%d", game_info->record);
	
	wrefresh(win);
}

void show_pixel(WINDOW * win, int y, int x, pixel_t pxl) 
{ 
	if(GET_VALUE(pxl)) mvwaddch(win, y + 1, x + 1, pxl); 
} 
