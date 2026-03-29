#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include "libft.h"
#include "render.h"

void	handleInput(t_render *render) {
	if (render->KEY[SDL_SCANCODE_ESCAPE])
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
	SDL_Surface*	surf1 = SDL_LoadBMP("./assets/button1.bmp");
	SDL_Surface*	surf2 = SDL_LoadBMP("./assets/button2.bmp");
	SDL_Surface*	surf3 = SDL_LoadBMP("./assets/button3.bmp");

	if (!surf1 || ! surf2 || ! surf3) {
		if (surf1) SDL_FreeSurface(surf1);
		if (surf2) SDL_FreeSurface(surf2);
		if (surf3) SDL_FreeSurface(surf3);
		ft_putendl_fd("ERROR\nSDL_LoadBMP failed", 2);
		destroy_render(render);
		return 1;
	}

	SDL_SetColorKey(surf1, SDL_TRUE, SDL_MapRGB(surf1->format, 255, 255, 255));
	SDL_SetColorKey(surf2, SDL_TRUE, SDL_MapRGB(surf2->format, 255, 255, 255));
	SDL_SetColorKey(surf3, SDL_TRUE, SDL_MapRGB(surf3->format, 255, 255, 255));

	render->button_texture[0] = SDL_CreateTextureFromSurface(render->ren, surf1);
	render->button_texture[1] = SDL_CreateTextureFromSurface(render->ren, surf2);
	render->button_texture[2] = SDL_CreateTextureFromSurface(render->ren, surf3);

	SDL_FreeSurface(surf1);
	SDL_FreeSurface(surf2);
	SDL_FreeSurface(surf3);

	if (!render->button_texture[0] || !render->button_texture[1] || !render->button_texture[2]) {
		ft_putendl_fd("ERROR\nSDL failed to create texture", 2);
		if (render->button_texture[0]) SDL_DestroyTexture(render->button_texture[0]);
		if (render->button_texture[1]) SDL_DestroyTexture(render->button_texture[1]);
		if (render->button_texture[2]) SDL_DestroyTexture(render->button_texture[2]);
		SDL_DestroyRenderer(render->ren);
		SDL_DestroyWindow(render->win);
		SDL_Quit();
		return 1;
	}
	return 0;
}
