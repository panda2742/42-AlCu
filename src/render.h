#ifndef RENDER_H
#define RENDER_H

#include <SDL2/SDL.h>
#include <sys/time.h>
#include "vector.h"

#define WINDOW_W 2500
#define WINDOW_H 1600

#define FRAME_W (WINDOW_W - 400)
#define FRAME_H 900
#define FRAME_X 200

#define SPRITE_W 80
#define SPRITE_H 64
#define MAX_ROTATIONS 8

typedef struct timeval	t_time;

typedef struct s_render
{
	SDL_Window		*win;
	SDL_Renderer	*ren;
	SDL_Texture		*texture;
	size_t			scroll_offset;
}	t_render;

typedef enum
{
	UP,
	DOWN,
}	t_scroll_dir;

t_render	*init_render(void);
void		destroy_render(t_render *render);
void		draw_frame(t_render *render, t_vector *game);

#endif