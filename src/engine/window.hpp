
#include <SDL.h>
#include <gl\glew.h>
#include <SDL_opengl.h>
#include <iostream>

/// Represents a single window
class Window {

	SDL_Window* handle = NULL;

public:

	Window(const char* title, int width, int height) {

		handle = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
		if (handle == NULL) {
			std::cout << "Window could not be created! SDL Error: " << SDL_GetError() << '\n';
		}
	}

	~Window() {
		//Destroy window	
		SDL_DestroyWindow(handle);
		handle = NULL;
	}

	bool isOk() {
		return this->handle != NULL;
	}

	/// Get SDL window object
	SDL_Window* getHandle() {
		return this->handle;
	}
};
