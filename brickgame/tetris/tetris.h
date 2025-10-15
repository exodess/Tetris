#ifndef TETRIS_H
#define TETRIS_H

#include "../../common/settings.h"

void start_game(ginfo_t * game_info);
int get_game_record(void);
void set_game_record(int);
void spawn(display_t *, ginfo_t *);
void processing_user_input(display_t *, ginfo_t *);
void pause(display_t * );
int interaction(display_t *, ginfo_t *);

int move_to_left(display_t *, ginfo_t *);
int move_to_right(display_t *, ginfo_t *);
int move_to_down(display_t *, ginfo_t *);
void rotate_shape(display_t *, ginfo_t *);

#endif
