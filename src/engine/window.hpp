
#include <SDL.h>

/// Represents a single window
class Window {

	SDL_Window* handle = NULL;

	public:

		Window( const char* title, int width, int height ) {
			handle = SDL_CreateWindow( title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN );
		}

		~Window() {
			SDL_DestroyWindow(handle);
		}

		bool isOk() {
			return this->handle != NULL;
		}

		/// Get SDL window object
		SDL_Window* getHandle() {
			return this->handle;
		}

};
