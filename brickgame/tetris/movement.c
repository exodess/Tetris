#include "tetris.h" 
#include "../../gui/cli/drowing.h" 
#include "../../common/settings.h"

int move_to_left(display_t * display_info, ginfo_t * game_info)
{
	/* движение влево осуществится только в том случае,
		если координата фигуры по оси x > 0 и слева нет препятствий для 
		движения в виде других блоков 

		Возвращает 1, если возможно движение фигуры влево, 
		           0, если движение влево невозможно
	*/
	int x = display_info->x;
	int result = (x > 0);
	if(result) {
		for(int i = 0; i < game_info->current_shape->height; i++)
			/* смотрим, чтобы в соседних пикселях не было препятствий */
			if(GET_VALUE(display_info->gfield[i][x - 1]) & GET_VALUE(game_info->current_shape->figure[i][0]))
				result = 0;
	} 

	return result;
}

int move_to_right(display_t * display_info, ginfo_t * game_info)
{
	/* Аналогично движению влево */
	
	int x = display_info->x;
	int width_shape = game_info->current_shape->width;
	int result = (x < (SCREEN_WIDTH - width_shape + 1));

	if(result) {
		for(int i = 0; i < game_info->current_shape->height; i++)
			if(GET_VALUE(display_info->gfield[i][x + width_shape]) & GET_VALUE(game_info->current_shape->figure[i][width_shape - 1]))
				result = 0;
	}

	return result;
}

int move_to_down(display_t * display_info, ginfo_t * game_info)
{
	/* Аналогично движению влево */

	int y = display_info->y;
	int width_shape = game_info->current_shape->width;
	int height_shape = game_info->current_shape->height;

	int result = (y < (SCREEN_HEIGHT - height_shape));

	if(result) {
		for(int i = 0; i < width_shape; i++)
			if(GET_VALUE(display_info->gfield[y + height_shape - 1][i]) & GET_VALUE(game_info->current_shape->figure[height_shape - 1][i]))
				result = 0;
	}

	return result;
	
}

void rotate_shape(display_t * display_info, ginfo_t * game_info)
{
	/* 
		Поворот фигуры осуществляется на 90 градусов влево
		
		Поворот фигуры влево происходит в два этапа:
			1. Выделение памяти под новую фигуру
			2. Копирование изначальной фигуры с ее поворотом влево
	*/

	shape_t new_shape;

	int width_newshape = game_info->current_shape->height;
	int height_newshape = game_info->current_shape->width;
	int x = display_info->x;
	int y = display_info->y;

	/* сначала необходимо проверить, 
		что новая фигура не выйдет за границы при повороте */
	if((x + width_newshape) < SCREEN_WIDTH && (y + height_newshape) < SCREEN_HEIGHT) {

		new_shape.height = height_newshape;
		new_shape.width = width_newshape;
		init_shape(&new_shape);	

		/* копирование с поворотом */

		for(int i = 0; i < height_newshape; i++)
			for(int j = 0; j < width_newshape; j++) 
				new_shape.figure[height_newshape - i - 1][j] = game_info->current_shape->figure[j][i];

		/* проверяем, чтобы фигура не конфликтовала с полем (не было столкновений) */

		int is_conflict = 0;

		for(int i = 0; i < height_newshape; i++)
			for(int j = 0; j < width_newshape; j++)
				if(GET_VALUE(display_info->gfield[y+i][x+j]) & GET_VALUE(new_shape.figure[i][j]))
					is_conflict = 1;

		if(!is_conflict) {
			/* переносим новые данные в исходный объект */
			game_info->current_shape->height = new_shape.height;
			game_info->current_shape->width = new_shape.width;

			/* освобождаем память под старую фигуру */
			destroy_shape(game_info->current_shape);
			game_info->current_shape->figure = new_shape.figure;
		}
		else destroy_shape(&new_shape);
	}

	
}
