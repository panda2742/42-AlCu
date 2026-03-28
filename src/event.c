#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include "libft.h"
#include "render.h"

void	handleInput(t_render *render) {
	if (render->KEY[SDLK_ESCAPE])
		render->running = SDL_FALSE;
}

void	drawButton(t_render *render, int x, int y, SDL_Texture* text) {
	SDL_Rect	dst = {
		.x = x,
		.y = y,
		.w = BUTTON_W,
		.h = BUTTON_H,
	};

	SDL_RenderCopy(render->ren, text, NULL, &dst);
}

bool	checkButton(int x, int y, int but_x, int but_y) {
	if (x >= but_x && x <= but_x + BUTTON_W)
		if (y >= but_y && y <= but_y + BUTTON_H)
			return 1;
	return 0;
}

int init_events(t_render *render) {
	SDL_Surface*	surf = SDL_LoadBMP("./asset/button.bmp");
	if (!surf) {
		ft_putendl_fd("ERROR\nSDL_LoadBMP failed", 2);
		destroy_render(render);
		return 1;
	}

	SDL_SetColorKey(surf, SDL_TRUE, SDL_MapRGB(surf->format, 255, 255, 255));

	render->button_texture = SDL_CreateTextureFromSurface(render->ren, surf);
	SDL_FreeSurface(surf);
	if (!render->button_texture) {
		ft_putendl_fd("ERROR\nSDL failed to create texture", 2);
		SDL_DestroyRenderer(render->ren);
		SDL_DestroyWindow(render->win);
		SDL_Quit();
		return 1;
	}
	return 0;
}
