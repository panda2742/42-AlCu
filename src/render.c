#include "render.h"
#include "vector.h"
#include "algo.h"

const char *decorations[] = {
	"./assets/decorations/tile_00.bmp",
	"./assets/decorations/tile_01.bmp",
	"./assets/decorations/tile_02.bmp",
	"./assets/decorations/tile_03.bmp",
	"./assets/decorations/tile_04.bmp",
	"./assets/decorations/tile_05.bmp",
	"./assets/decorations/tile_06.bmp",
	"./assets/decorations/tile_07.bmp",
	"./assets/decorations/tile_08.bmp",
	"./assets/decorations/tile_09.bmp",
	"./assets/decorations/tile_10.bmp",
	"./assets/decorations/tile_11.bmp",
	"./assets/decorations/tile_12.bmp",
	"./assets/decorations/tile_13.bmp",
	"./assets/decorations/tile_14.bmp",
	"./assets/decorations/tile_15.bmp",
	"./assets/decorations/tile_16.bmp",
	"./assets/decorations/tile_17.bmp",
	"./assets/decorations/tile_18.bmp",
	"./assets/decorations/tile_19.bmp",
	"./assets/decorations/tile_20.bmp",
	"./assets/decorations/tile_21.bmp",
	"./assets/decorations/tile_22.bmp",
	"./assets/decorations/tile_23.bmp",
	"./assets/decorations/tile_24.bmp",
	"./assets/decorations/tile_25.bmp",
	"./assets/decorations/tile_26.bmp",
	"./assets/decorations/tile_27.bmp",
	"./assets/decorations/tile_28.bmp",
	"./assets/decorations/tile_29.bmp",
	"./assets/decorations/tile_30.bmp",
	"./assets/decorations/tile_31.bmp",
};

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

	surf = SDL_LoadBMP("./assets/grass.bmp");
	if (!surf) {
		__builtin_printf("Cannot init surface.");
		destroy_render(render);
		return NULL;
	}

	render->grass_texture = SDL_CreateTextureFromSurface(render->ren, surf);
	SDL_FreeSurface(surf);
	if(!render->texture)
	{
		__builtin_printf("Cannot init texture.");
		destroy_render(render);
		return NULL;
	}

	for (int i = 0; i < 32; ++i) {
		surf = SDL_LoadBMP(decorations[i]);
		if (!surf) {
			__builtin_printf("Cannot init surface.");
			destroy_render(render);
			return NULL;
		}

		render->decorations[i] = SDL_CreateTextureFromSurface(render->ren, surf);
		SDL_FreeSurface(surf);
	}

	return render;
}

void	destroy_render(t_render *render)
{
	if (!render) return;

	if (render->texture) SDL_DestroyTexture(render->texture);
	if (render->button_texture[0]) SDL_DestroyTexture(render->button_texture[0]);
	if (render->button_texture[1]) SDL_DestroyTexture(render->button_texture[1]);
	if (render->button_texture[2]) SDL_DestroyTexture(render->button_texture[2]);
	if (render->ren) SDL_DestroyRenderer(render->ren);
	if (render->win) SDL_DestroyWindow(render->win);
	SDL_Quit();
	free(render);
}

void	render_background(t_render *render)
{
	static t_bool	generated = FALSE;
	static int		generated_value = 0;
	if (generated == FALSE) {
		generated = TRUE;
		generated_value = rand();
	}

	SDL_RenderCopy(render->ren, render->grass_texture, NULL, NULL);

	int	y_amount = WIN_H / DECO_H / 6;
	int	x_amount = WIN_W / DECO_W / 6;

	for (int i = 0; i < y_amount; ++i) {
		for (int j = 0; j < x_amount; ++j) {
			SDL_Rect src = {
				.x = 0,
				.y = 0,
				.w = DECO_W,
				.h = DECO_H
			};

			SDL_Rect dst = {
				.x = j * (DECO_W * 6),
				.y = i * (DECO_H * 6),
				.w = DECO_W,
				.h = DECO_H
			};
			SDL_RenderCopy(render->ren, render->decorations[(i * j % 234297852 * generated_value) % 32], &src, &dst);
		}
	}
}


void	draw_frame(t_render *render, t_vector *game)
{
	render_background(render);
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