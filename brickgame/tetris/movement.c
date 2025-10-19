#include "tetris.h" 
#include "../../gui/cli/drowing.h" 
#include "../../common/settings.h"
#include <stdlib.h>

int move_to_left(display_t * display_info, ginfo_t * game_info)
{
	/* движение влево осуществится только в том случае,
		если координата фигуры по оси x > 0 и слева нет препятствий для 
		движения в виде других блоков 

		Возвращает 1, если возможно движение фигуры влево, 
		           0, если движение влево невозможно
	*/
	int x = display_info->x;
	int y = display_info->y;
	int width_shape = game_info->current_shape->width;
	int height_shape = game_info->current_shape->height;
	
	int result = (x > 0);
	
	if(result) {
		for(int i = 0; i < height_shape; i++)
			for(int j = 0; j < width_shape; j++)
			/* смотрим, чтобы в соседних пикселях не было препятствий */
				if(GET_VALUE(display_info->gfield[y + i][x + j - 1]) & \
					 GET_VALUE(game_info->current_shape->figure[i][j]))
					result = 0;
	} 

	return result;
}

int move_to_right(display_t * display_info, ginfo_t * game_info)
{
	/* Аналогично движению влево */
	
	int x = display_info->x;
	int y = display_info->y;
	int width_shape = game_info->current_shape->width;
	int height_shape = game_info->current_shape->height;
	int result = (x < (SCREEN_WIDTH - width_shape + 1));

	if(result) {
		for(int i = 0; i < height_shape; i++)
			for(int j = 0; j < width_shape; j++)
				if(GET_VALUE(display_info->gfield[y + i][x + j + 1]) & \
					 GET_VALUE(game_info->current_shape->figure[i][j]))
					result = 0;
	}

	return result;
}

int move_to_down(display_t * display_info, ginfo_t * game_info)
{
	/* Аналогично движению влево */

	int y = display_info->y;
	int x = display_info->x;
	int width_shape = game_info->current_shape->width;
	int height_shape = game_info->current_shape->height;

	int result = (y < (SCREEN_HEIGHT - height_shape));

	if(result) {
		for(int i = 0; i < height_shape; i++)
			for(int j = 0; j < width_shape; j++)
				if(GET_VALUE(display_info->gfield[y + i + 1][x+j]) & \
					 GET_VALUE(game_info->current_shape->figure[i][j]))
					result = 0;
	}

	return result;
	
}

void rotate_shape(display_t * display_info, ginfo_t * game_info)
{
	/* 
		Поворот фигуры осуществляется на 90 градусов влево относительно центра масс
		
		Поворот фигуры влево происходит в два этапа:
			1. Выделение памяти под новую фигуру
			2. Копирование изначальной фигуры с ее поворотом влево

		При повороте следует учитывать, не будет ли конфликтовать фигура с препятствиями
			на игровом поле (не будет ли наложения фигуры на препятствия)
	*/

	shape_t new_shape;

	/* размеры фигуры до поворота */
	int width_oldshape = game_info->current_shape->width;
	int height_oldshape = game_info->current_shape->height;

	/* размеры фигуры после поворота */
	int width_newshape = height_oldshape;
	int height_newshape = width_oldshape;

	/* текущие координаты фигуры на поле */
	int x = display_info->x;
	int y = display_info->y;

	/* смещение фигуры после поворота (изменение ее координат) */
	int det_x = (width_newshape - width_oldshape) / 2;
	int det_y = (height_newshape - height_oldshape) / 2;
	

	/* сначала необходимо проверить, 
		что новая фигура не выйдет за границы при повороте */
	if((x - det_x + width_newshape) < SCREEN_WIDTH && (y - det_y + height_newshape) < SCREEN_HEIGHT) {

		new_shape.height = height_newshape;
		new_shape.width = width_newshape;
		init_shape(&new_shape);	
		/* копирование с поворотом */

		for(int i = 0; i < height_newshape; i++)
			for(int j = 0; j < width_newshape; j++) 
				new_shape.figure[(height_newshape - i - 1)%height_newshape][j] = game_info->current_shape->figure[j][i];

		/* проверяем, чтобы фигура не конфликтовала с полем (не было столкновений) */

		int is_conflict = 0;

		for(int i = 0; i < height_newshape; i++)
			for(int j = 0; j < width_newshape; j++)
				if(GET_VALUE(display_info->gfield[y - det_y + i][x - det_x + j]) & GET_VALUE(new_shape.figure[i][j]))
					is_conflict = 1;

		if(!is_conflict) {
			/* поворот прошел успешно */
			
			/* освобождаем память под старую фигуру */
			destroy_shape(game_info->current_shape);

			/* переносим новые данные в исходный объект */
			game_info->current_shape->height = new_shape.height;
			game_info->current_shape->width = new_shape.width;
			game_info->current_shape->figure = new_shape.figure;

			/* изменяем координаты */
			display_info->x -= det_x;
			display_info->y -= det_y;
		}
		/* при повороте возникает наложение */
		else destroy_shape(&new_shape);

	}

	

}
