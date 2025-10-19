#include <stdio.h>
#include <time.h> 
#include <stdlib.h>
#include <pthread.h>
#define _BSD_SOURCE /* для usleep */
#include <unistd.h>
#include "tetris.h"
#include "../../gui/cli/drowing.h"

int main(void)
{
	/* главная функция программы */

	/* инициализируем структуру экрана игрового поля */
	int is_exit = 0;
	display_t display_info;
	ginfo_t game_info;

	init_game_field(&display_info);

	while(!is_exit) {
		/* реализуем конечный автомат */
		/* храним игровую информацию об очках и уровне */

		int is_end = 0;
		
		start_game(&game_info);

		while(!is_end) {
			spawn(&display_info, &game_info);

			processing_user_input(&display_info, &game_info);

			is_end = interaction(&display_info, &game_info);
		}

		is_exit = end_game(&display_info, &game_info);
		
	}

	destroy_game_field(&display_info);

	return 0;
}

void spawn_shape(shape_t * this)
{
	/* Генерируем случайную фигуру и создаем ее */

	srand(time(0)); 
	int new_type = rand() % COUNT_SHAPES; 
	
	create_shape(this, new_type);

}

void spawn(display_t * display_info, ginfo_t * game_info)
{
	/* next_shape становится current_shape, 
		для next_shape создается новая фигура и сохраняется в памяти */

	display_info->y = 0;
	display_info->x = SCREEN_WIDTH / 2 - 1;

	shape_t *cur = game_info->current_shape, *next = game_info->next_shape;

	/* удаляем текущую фигуру */
	destroy_shape(cur);

	/* копируем информацию о следущей фигуре, которая теперь стала текущей */
	cur->height = next->height;
	cur->width = next->width;
	cur->figure = next->figure;

	/* создаем следующую фигуру */
	spawn_shape(next);
	
}

void * pdelay(void * arg)
{
	/* реализуем задержку в виде отдельного потока */

	unsigned int time_delay = *(unsigned int*)arg;

	usleep(time_delay); 

	return NULL;
}

void processing_user_input(display_t * display_info, ginfo_t * game_info) 
{ 
	chtype ch; 
	int is_down = 0;
	pthread_t ptdelay;

	unsigned int time_delay = GET_HALFDELAY(game_info->level); 

	while(!is_down) { 
		/* засекаем время, которое фигура не будет падать вниз (задержка) */

		/* создаем отдельный поток для задержки вывода */
		pthread_create(&ptdelay, NULL, pdelay, &time_delay);
	
		display(display_info, game_info);
		
		while(!pthread_join(ptdelay, NULL)) {

			ch = getch();
			if(ch == KEY_PAUSE || ch == KEY_TO_LEFT || ch == KEY_TO_RIGHT || \
			   ch == KEY_TO_DOWN || ch == KEY_ROTATE || ch == KEY_SHOW_NEXT_FIGURE) {
			   	
				if(ch == KEY_PAUSE) game_pause(display_info);
								
				else if(ch == KEY_TO_LEFT) 
					display_info->x -= move_to_left(display_info, game_info);
					
				else if(ch == KEY_TO_RIGHT)
					display_info->x += move_to_right(display_info, game_info);
					
				else if(ch == KEY_TO_DOWN)
					/* фигура падает вниз */
					while(move_to_down(display_info, game_info)) display_info->y ++;
					
				else if(ch == KEY_ROTATE)
					rotate_shape(display_info, game_info);
					
				else if(ch == KEY_SHOW_NEXT_FIGURE)
					switch_next_shape_window(display_info);	

				display(display_info, game_info);

			}

			while(getch() != ERR) ; 

		}
		
		if(!move_to_down(display_info, game_info)) is_down = 1;
		if(!is_down) display_info->y++;
		
	} 

} 

int interaction(display_t * display_info, ginfo_t * game_info) 
{
	/* обработка положения, когда фигура упала на дно 
		Возвращает 1, если нужно закончить игру (невозможно создать новую фигуру,
			так поле заполнено)
	*/

	int y = display_info->y;
	int x = display_info->x;

	/* присваиваем упавшую фигуру игровому полю */
	for(int i = 0; i < game_info->current_shape->height; i++)
		for(int j = 0; j < game_info->current_shape->width; j++)
			display_info->gfield[y + i][x + j] |= game_info->current_shape->figure[i][j];

	/* проверяем, есть ли полностью заполненные линии */
	int count_full_lines = 0;

	for(int i = SCREEN_HEIGHT - 1; i >= 0; i--) {
		int is_full = 1;

		for(int j = 0; j < SCREEN_WIDTH; j++)
			if(!GET_VALUE(display_info->gfield[i][j])) is_full = 0;

		if(is_full) {
			count_full_lines++;
			remove_full_line(display_info, i);
			i++;
		}
	}
	/* обновляем значение счетчика очков */
	game_info->current_score += GET_POINTS_FOR_DESTROY_LINE(count_full_lines);

	/* обновляем рекорд в игре */
	if(game_info->current_score > get_game_record()) {
		game_info->record = game_info->current_score;
		set_game_record(game_info->record);
	}

	/* обновляем уровень, если набралось нужное количество очков */
	game_info->level = game_info->current_score / COUNT_POINTS_TO_LEVEL_UP;
		
	display(display_info, game_info);

	return (y == 0);
}

void remove_full_line(display_t * display_info, int y)
{
	/* функция служит для удаления заполненной строки из игрового поля
		Удаление происходит путем сдвига верхних строк на одну позицию вниз
	 */

	 
	for(int i = y; i > 0; i--)
		for(int j = 0; j < SCREEN_WIDTH; j++)
			display_info->gfield[i][j] = display_info->gfield[i - 1][j];

	/* самая верхняя линия становится пустой */
	for(int j = 0; j < SCREEN_WIDTH; j++)
		display_info->gfield[0][j] = '0';
	
}

void game_pause(display_t * display_info)
{
	chtype ch; 
 	/* выводим слово PAUSE */
	drowing_pause_place(display_info, 0);
	
	while((ch = getch()) != KEY_START_GAME) ; 

	/* очищаем слово PAUSE */
	drowing_pause_place(display_info, 1);
}



void start_game(ginfo_t * game_info)
{
	/* Выводим начальное окно приветствия и ожидаем,
		пока пользователь не нажмет кнопку готовности к игре 

		Параллельно устанавливаем игровые показатели и время обновления экрана
	*/
	
	game_info->level = 0;
	game_info->record = get_game_record();
	game_info->current_score = 0;

	game_info->next_shape = malloc(sizeof(shape_t));
	game_info->current_shape = malloc(sizeof(shape_t));
	
	spawn_shape(game_info->next_shape); 

	drowing_start_place();
		
}

int end_game(display_t * display_info, ginfo_t * game_info)
{
	free(game_info->next_shape);
	free(game_info->current_shape);
	game_info->next_shape = NULL;
	game_info->current_shape = NULL;

	/* очищаем игровое поле */
	for(int i = 0; i < SCREEN_HEIGHT; i++)
		for(int j = 0; j < SCREEN_WIDTH; j++)
			display_info->gfield[i][j] = 0;

	return drowing_exit_place(game_info);
}

int get_game_record()
{
	/* текущий игровой рекорд находится в файле src/bin/.save */
	int record = 0;
	FILE * fsave = fopen(SAVE_FILE, "r");
	if(fsave) { 
		/* файл существует */
		fscanf(fsave, "%d", &record);

		fclose(fsave);
	}
	else 
		/* иначе создадим его и назначим рекорд как 0 */
		set_game_record(0);
	
	return record;
}

void set_game_record(int new_record)
{
	FILE * fsave = fopen(SAVE_FILE, "w");

	fprintf(fsave, "%d", new_record);

	fclose(fsave);
}
