#include "render.h"
#include "vector.h"
#include "algo.h"

t_render	*init_render(void)
{
	t_render	*render = malloc(sizeof(t_render));
	if (!render)
	{
		__builtin_printf("Cannot init SDL.");
		return NULL;
	}

	render->win = NULL;
	render->ren = NULL;
	render->texture = NULL;
	render->scroll_offset = 0;
	render->running = SDL_TRUE;
	render->choice = 0;
	for (int i = 0; i < 322; ++i) {
		render->KEY[i] = 0;
	}

	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		__builtin_printf("Cannot init SDL.");
		return NULL;
	}

	render->win = SDL_CreateWindow(
		"AlCats",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        WIN_W, WIN_H,
        SDL_WINDOW_SHOWN
	);
	if (!render->win)
	{
		__builtin_printf("Cannot init window.");
		destroy_render(render);
		return NULL;
	}

	render->ren = SDL_CreateRenderer(
		render->win,
		-1,
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
	);
	if (!render->ren)
	{
		__builtin_printf("Cannot init renderer.");
		destroy_render(render);
		return NULL;
	}

	SDL_Surface	*surf = SDL_LoadBMP("./assets/cat.bmp");
	if (!surf) {
		__builtin_printf("Cannot init surface.");
		destroy_render(render);
		return NULL;
	}

	render->texture = SDL_CreateTextureFromSurface(render->ren, surf);
	SDL_FreeSurface(surf);
	if(!render->texture)
	{
		__builtin_printf("Cannot init texture.");
		destroy_render(render);
		return NULL;
	}

	return render;
}

void	destroy_render(t_render *render)
{
	if (!render) return;

	if (render->texture) SDL_DestroyTexture(render->texture);
	if (render->button_texture) SDL_DestroyTexture(render->button_texture);
	if (render->ren) SDL_DestroyRenderer(render->ren);
	if (render->win) SDL_DestroyWindow(render->win);
	SDL_Quit();
	free(render);
}

void	draw_frame(t_render *render, t_vector *game)
{
	int	k = 0;
	for (ssize_t i = game->size - 1 - render->scroll_offset; i >= 0; --i) {
		const int	y = FRAME_H - (SPRITE_H * k++);
		if (y + SPRITE_H < 0) break;

		const int	n = ((int *)game->tab)[i];
		for (int j = 0; j < n; j++) {
			SDL_Rect src = {
				.x = ((j * n) % 8) * SPRITE_W,
				.y = 0,
				.w = SPRITE_W,
				.h = SPRITE_H
			};

			SDL_Rect dst = {
				.x = j * SPRITE_W + FRAME_X,
				.y = y,
				.w = SPRITE_W,
				.h = SPRITE_H
			};
			SDL_RenderCopy(render->ren, render->texture, &src, &dst);
		}
	}
}

void	scroll(t_render *render, size_t lanes, t_scroll_dir dir)
{
	const int	scroll_value = dir == UP ? -1 : 1;

	if (render->scroll_offset + scroll_value < 0) return;
	render->scroll_offset = max(0, min(scroll_value + render->scroll_offset, lanes - 1));
}
