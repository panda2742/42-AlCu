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

static t_bool	generated = FALSE;
static int		generated_value[100000];

t_render	*init_render(void)
{
	t_render	*render = malloc(sizeof(t_render));
	if (!render)
	{
		ft_putstr_fd("Cannot init SDL.\n", 2);
		return NULL;
	}

	render->win = NULL;
	render->ren = NULL;
	render->texture = NULL;
	render->grass_texture = NULL;
	render->scroll_offset = 0;
	render->running = SDL_TRUE;
	render->choice = 0;
	render->last_update_i = 0;
	gettimeofday(&render->last_update, NULL);
	for (int i = 0; i < SDL_NUM_SCANCODES; ++i) {
		render->KEY[i] = 0;
	}
	for (int i = 0; i < 3; ++i) {
		render->button_texture[i] = NULL;
	}
	for (int i = 0; i < 32; ++i) {
		render->decorations[i] = NULL;
	}

	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		ft_putstr_fd("Cannot init SDL.\n", 2);
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
		ft_putstr_fd("Cannot init window.\n", 2);
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
		ft_putstr_fd("Cannot init renderer.\n", 2);
		destroy_render(render);
		return NULL;
	}

	SDL_Surface	*surf = SDL_LoadBMP("./assets/cat.bmp");
	if (!surf) {
		ft_putstr_fd("Cannot init surface.\n", 2);
		destroy_render(render);
		return NULL;
	}

	render->texture = SDL_CreateTextureFromSurface(render->ren, surf);
	SDL_FreeSurface(surf);
	if(!render->texture)
	{
		ft_putstr_fd("Cannot init textures.\n", 2);
		destroy_render(render);
		return NULL;
	}

	surf = SDL_LoadBMP("./assets/grass.bmp");
	if (!surf) {
		ft_putstr_fd("Cannot init surface.\n", 2);
		destroy_render(render);
		return NULL;
	}

	render->grass_texture = SDL_CreateTextureFromSurface(render->ren, surf);
	SDL_FreeSurface(surf);
	if(!render->texture)
	{
		ft_putstr_fd("Cannot init texture.\n", 2);
		destroy_render(render);
		return NULL;
	}

	for (int i = 0; i < 32; ++i) {
		surf = SDL_LoadBMP(decorations[i]);
		if (!surf) {
			ft_putstr_fd("Cannot init surface.\n", 2);
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
	if (render->grass_texture) SDL_DestroyTexture(render->grass_texture);
	if (render->button_texture[0]) SDL_DestroyTexture(render->button_texture[0]);
	if (render->button_texture[1]) SDL_DestroyTexture(render->button_texture[1]);
	if (render->button_texture[2]) SDL_DestroyTexture(render->button_texture[2]);
	for (int i = 0; i < 32; i++) {
		if (render->decorations[i]) SDL_DestroyTexture(render->decorations[i]);
	}
	if (render->ren) SDL_DestroyRenderer(render->ren);
	if (render->win) SDL_DestroyWindow(render->win);
	SDL_Quit();
	free(render);
}

void	render_background(t_render *render)
{
	if (generated == FALSE) {
		generated = TRUE;
		for (int i = 0; i < 100000; ++i)
		{
			generated_value[i] = rand() % 32;
		}
	}

	SDL_RenderCopy(render->ren, render->grass_texture, NULL, NULL);

	t_time	now;
	gettimeofday(&now, NULL);
	int	time_diff = now.tv_sec - render->last_update.tv_sec - (now.tv_usec - render->last_update.tv_usec) * 1000000;
	if (time_diff >= 200000)
	{
		render->last_update.tv_sec = now.tv_sec;
		render->last_update.tv_usec = now.tv_usec;
		render->last_update_i = (render->last_update_i + 1) % 8;
	}

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
			SDL_RenderCopy(render->ren, render->decorations[generated_value[(i * j) % 100000]], &src, &dst);
		}
	}
}


void	draw_frame(t_render *render, t_vector *game)
{
	if (generated == FALSE) {
		generated = TRUE;
		for (int i = 0; i < 100000; ++i)
		{
			generated_value[i] = rand() % 32;
		}
	}

	render_background(render);
	int	k = 0;
	for (ssize_t i = game->size - 1 - render->scroll_offset; i >= 0; --i) {
		const int	y = FRAME_H - (SPRITE_H * k++);
		if (y + SPRITE_H < 0) break;

		const int	n = ((int *)game->tab)[i];
		for (int j = 0; j < n; j++) {
			const int x = j * SPRITE_W + FRAME_X;
			if (x > WIN_W) break;

			SDL_Rect src = {
				.x = ((generated_value[(i * j) % 100000] + render->last_update_i) % 8) * SPRITE_W,
				.y = 0,
				.w = SPRITE_W,
				.h = SPRITE_H
			};

			SDL_Rect dst = {
				.x = x,
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