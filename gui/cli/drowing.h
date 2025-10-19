#ifndef DROWING_H
#define DROWING_H

#include  "../../common/settings.h"

#define GET_VALUE(pxl) ((char) pxl > '0')

/* memory display managment */
void init_game_field(display_t *);
void destroy_game_field(display_t *);
void init_shape(shape_t *);
void create_shape(shape_t *, int);
void destroy_shape(shape_t *);
void switch_next_shape_window(display_t * display_info);

/* drowing interface */
void show_pixel(WINDOW *, int x, int y, pixel_t pxl);
void drowing_game_field(display_t *, ginfo_t *);
void drowing_sidebar(display_t *, ginfo_t *);
void drowing_panel_with_next_shape(display_t *, ginfo_t *);
void drowing_start_place(void);
void display(display_t *, ginfo_t *);
void drowing_pause_place(display_t *, int);
int drowing_exit_place(ginfo_t *);

#endif
