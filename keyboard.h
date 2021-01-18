#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <SDL2/SDL.h>

enum OS_KEYBOARD_TYPE
{
	QUIT = SDL_QUIT,
	KEY_DOWN = SDL_KEYDOWN,
};

enum OS_KEYBOARD_KEY {
	BT_ARROW_LEFT = SDLK_LEFT,
	BT_ARROW_RIGHT = SDLK_RIGHT,
	BT_ARROW_UP = SDLK_UP,
	BT_ARROW_DOWN = SDLK_DOWN,
	BT_KEY_P = SDLK_p,
	BT_KEY_O = SDLK_o,
	BT_KEY_S = SDLK_s,
	BT_KEY_D = SDLK_d,
	BT_KEY_A = SDLK_a,
	BT_KEY_B = SDLK_b,
};


#endif // !KEYBOARD_H
