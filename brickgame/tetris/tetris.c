#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "tetris.h"
#include "../../gui/cli/drowing.h"
/* #include "../../common/settings.h" */



int main(void)
{
	/* главная функция программы */

	/* инициализируем структуру экрана игрового поля */

	display_t display_info;
	ginfo_t game_info;

	init_game_field(&display_info);

	while(1) {
		/* реализуем конечный автомат */
		/* храним игровую информацию об очках и уровне */

		int is_end = 0;
		
		start_game(&game_info);

		while(!is_end) {
			spawn(&display_info, &game_info);

			processing_user_input(&display_info, &game_info);

			is_end = interaction(&display_info, &game_info);
							
		}
		
	}

	destroy_game_field(&display_info);

	return 0;
}

void spawn_shape(shape_t * this)
{
	/* Генерируем случайную фигуру и создаем ее */
	srand(time(0));
	int new_type = rand() % COUNT_SHAPES;
	if(this) printf("%d", new_type);
	create_shape(this, new_type); 

}

void spawn(display_t * display_info, ginfo_t * game_info)
{
	/* next_shape становится current_shape, 
		для next_shape создается новая фигура и сохраняется в памяти */

	display_info->y = 0;
	display_info->x = SCREEN_WIDTH / 2;

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

void processing_user_input(display_t * display_info, ginfo_t * game_info) 
{ 
	chtype ch; 
	int is_down = 0;
	display(display_info, game_info);
	
	while(!is_down && (ch = getch()) != (unsigned)ERR) { 
		
	
		if(ch == KEY_PAUSE) pause(display_info); 

		else if(ch == KEY_TO_LEFT) 
			display_info->x -= move_to_left(display_info, game_info);
			
		else if(ch == KEY_TO_RIGHT)
			display_info->x += move_to_right(display_info, game_info);
			
		else if(ch == KEY_TO_DOWN)
			/* фигура падает вниз */
			while(move_to_down(display_info, game_info)) display_info->y ++;
			
		else if(ch == KEY_ROTATE)
			rotate_shape(display_info, game_info);
			
		else if(ch == 'h')
			printf("Показ следующей фигуры");

		if(!move_to_down(display_info, game_info)) is_down = 1;
		if(!is_down) display_info->y++;

		display(display_info, game_info);
		
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
			display_info->gfield[y + i][x + j] = game_info->current_shape->figure[i][j];

	/* проверяем, есть ли полностью заполненные линии */
	int count_full_lines = 0;

	for(int i = SCREEN_HEIGHT - 1; i >= 0; i--) {
		int is_full = 1;

		for(int j = 0; j < SCREEN_WIDTH; j++)
			if(!GET_VALUE(display_info->gfield[i][j])) is_full = 0;

		count_full_lines += is_full;
	}

	game_info->current_score += GET_POINTS_FOR_DESTROY_LINE(count_full_lines);

	if(game_info->current_score > get_game_record()) {
		game_info->record = game_info->current_score;
		set_game_record(game_info->record);
	}
	
	if(game_info->current_score > COUNT_POINTS_TO_LEVEL_UP && game_info->level < COUNT_LEVELS)
		game_info->level++; 


	return (y == 0);
}

void pause(display_t * display_info)
{
	chtype ch; 
	drowing_pause_place(display_info);
	
	nocbreak(); 
	while((char)(ch = getch()) != KEY_START_GAME) ; 

	halfdelay(INITIAL_DELAY); 
}



void start_game(ginfo_t * game_info)
{
	/* Выводим начальное окно приветствия и ожидаем,
		пока пользователь не нажмет кнопку готовности к игре 

		Параллельно устанавливаем игровые показатели и время обновления экрана
	*/

	nocbreak();
	game_info->level = 0;
	game_info->record = get_game_record();
	game_info->current_score = 0;

	game_info->next_shape = malloc(sizeof(shape_t));
	game_info->current_shape = malloc(sizeof(shape_t));
	
	spawn_shape(game_info->next_shape); 

	drowing_start_place();
	halfdelay(INITIAL_DELAY);
	
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
