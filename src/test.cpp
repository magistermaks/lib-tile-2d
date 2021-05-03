
#include "engine/engine.cpp"
#include <string>

// playground for testing things
// run with python3 build.py --test
// this will be removed in the future in favor of test written in Java


int main(int argc, char* argv[]) {

	if (!Engine::lib_init()) return 0;
	Window window("Hello!", 1000, 1000);
	Layer layer(100, 100);
	layers.push_back(&layer);
	if (!Engine::lib_initGL(&window)) return 0;

	if (window.isOk()) {
		bool quit = false;

		//Enable text input
		SDL_StartTextInput();

		while (!quit) {
			Engine::render();

			//Update screen
			SDL_GL_SwapWindow(window.getHandle());
		}
	}

	Engine::lib_exit();
	return 0;
}
