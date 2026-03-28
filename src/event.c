#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include "libft.h"

#define WIN_H 1080
#define WIN_W 1920
#define BUTTON_W 256
#define BUTTON_H 128

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

bool			KEY[322];
SDL_bool		running = SDL_TRUE;
SDL_Renderer*	ren;
int				move = 0;
int				choice = 0;

void	handleInput() {
	if (KEY[SDLK_ESCAPE])
		running = SDL_FALSE;
}

void	drawButton(int x, int y, SDL_Texture* text) {
	SDL_Rect	dst = {
		.x = x,
		.y = y,
		.w = BUTTON_W,
		.h = BUTTON_H,
	};

	SDL_RenderCopy(ren, text, NULL, &dst);
}

bool	checkButton(int x, int y, int but_x, int but_y) {
	if (x >= but_x && x <= but_x + BUTTON_W)
		if (y >= but_y && y <= but_y + BUTTON_H)
			return 1;
	return 0;
}

int main(void) {
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		ft_putendl_fd("ERROR\nFailed to init SDL2", 2);
		return 1;
	}

	SDL_Window*	win = SDL_CreateWindow("TEST", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIN_W, WIN_H, SDL_WINDOW_SHOWN);
	if (!win) {
		ft_putendl_fd("ERROR\nFailed to create window", 2);
		SDL_Quit();
		return 1;
	}

	ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (!ren) {
		ft_putendl_fd("ERROR\nFailed to create render", 2);
		SDL_DestroyWindow(win);
		SDL_Quit();
		return 1;
	}

	SDL_Surface*	surf = SDL_LoadBMP("./asset/button.bmp");
	if (!surf) {
		ft_putendl_fd("ERROR\nSDL_LoadBMP failed", 2);
		SDL_DestroyRenderer(ren);
		SDL_DestroyWindow(win);
		SDL_Quit();
		return 1;
	}

	SDL_SetColorKey(surf, SDL_TRUE, SDL_MapRGB(surf->format, 255, 255, 255));

	SDL_Texture*	text = SDL_CreateTextureFromSurface(ren, surf);
	SDL_FreeSurface(surf);
	if (!text) {
		ft_putendl_fd("ERROR\nSDL failed to create texture", 2);
		SDL_DestroyRenderer(ren);
		SDL_DestroyWindow(win);
		SDL_Quit();
		return 1;
	}

	SDL_Event	ev;
	while (running) {
		while (SDL_PollEvent(&ev)) {
			switch (ev.type) {
				int x, y;
				case SDL_QUIT:
					running = SDL_FALSE;
					break;
				case SDL_KEYDOWN:
					KEY[ev.key.keysym.sym] = true;
					break;
				case SDL_KEYUP:
					KEY[ev.key.keysym.sym] = false;
					break;
				case SDL_MOUSEBUTTONDOWN:
					SDL_GetMouseState(&x, &y);
					if (checkButton(x, y, BUTTON_1))
						choice = 1;
					else if (checkButton(x, y, BUTTON_2))
						choice = 2;
					else if (checkButton(x, y, BUTTON_3))
						choice = 3;
					break;
				case SDL_MOUSEWHEEL:
					if (ev.wheel.y > 0)
						move = 1;
					else
						move = -1;
			}
			handleInput();
		}
		if (choice) {
			ft_putstr_fd("Choice: ", 1);
			ft_putnbr_fd(choice, 1);
			write(1, "\n", 1);
			choice = 0;
		}

		SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
		SDL_RenderClear(ren);
		drawButton(BUTTON_1, text);
		drawButton(BUTTON_2, text);
		drawButton(BUTTON_3, text);
		SDL_RenderPresent(ren);
		SDL_Delay(300);
	}
}
