
#include "engine/engine.cpp"

// playground for testing things
// run with python3 build.py --test
int main() {

	lib_init();
	Window window( "Hello!", 300, 300 );

	if( window.isOk() ) {

		SDL_Surface* surface = SDL_GetWindowSurface( window.getHandle() );

		//Fill the surface white
		SDL_FillRect( surface, NULL, SDL_MapRGB( surface->format, 0xFF, 0xFF, 0xFF ) );

		//Update the surface
		SDL_UpdateWindowSurface( window.getHandle() );

		// wait 4 seconds
		SDL_Delay( 4000 );
	}

	lib_exit();

}
