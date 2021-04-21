
#include <SDL.h>
#include "window.hpp"

/// Initialize LibTile2D
bool lib_init() {
	return SDL_Init( SDL_INIT_VIDEO ) >= 0;
}

/// Exit LibTile2D
void lib_exit() {
	SDL_Quit();
}
