#ifndef RENDER_H
#define RENDER_H

#include <SDL2/SDL.h>
#include <sys/time.h>
#include "vector.h"


#define WIN_W 2500
#define WIN_H 1600
#define BUTTON_W 256
#define BUTTON_H 128
#define FRAME_W (WIN_H - 400)
#define FRAME_H 900
#define FRAME_X 200

#define BUT_HGT_RATIO 5 / 6

#define BUTTON_1_X WIN_W / 6 - BUTTON_W / 2
#define BUTTON_1_Y WIN_H * BUT_HGT_RATIO - BUTTON_H / 2
#define BUTTON_2_X WIN_W / 2 - BUTTON_W / 2
#define BUTTON_2_Y WIN_H * BUT_HGT_RATIO - BUTTON_H / 2
#define BUTTON_3_X WIN_W * 5 / 6 - BUTTON_W / 2
#define BUTTON_3_Y WIN_H * BUT_HGT_RATIO - BUTTON_H / 2

#define BUTTON_1 BUTTON_1_X, BUTTON_1_Y
#define BUTTON_2 BUTTON_2_X, BUTTON_2_Y
#define BUTTON_3 BUTTON_3_X, BUTTON_3_Y

#define SPRITE_W 80
#define SPRITE_H 64
#define MAX_ROTATIONS 8

typedef struct timeval	t_time;

typedef struct s_render
{
	SDL_Window		*win;
	SDL_Renderer	*ren;
	SDL_Texture		*texture;
	SDL_Texture		*button_texture;
	ssize_t			scroll_offset;
	SDL_bool		running;
	bool			KEY[322];
	int				choice;
}	t_render;

typedef enum
{
	UP,
	DOWN,
}	t_scroll_dir;

t_render	*init_render(void);
void		destroy_render(t_render *render);
void		draw_frame(t_render *render, t_vector *game);
void		handleInput(t_render *render);
void		drawButton(t_render *render, int x, int y, SDL_Texture* text);
bool		checkButton(int x, int y, int but_x, int but_y);
void		scroll(t_render *render, size_t lanes, t_scroll_dir dir);

int init_events(t_render *render);


#endif