
#include "window.hpp"

extern Window* window;

/// Initialize LibTile2D
bool lib_init() {
	return SDL_Init( SDL_INIT_VIDEO ) >= 0;
}

/// Exit LibTile2D
void lib_exit() {
	SDL_Quit();
}

void render() {
	//Clear color buffer
	glClear(GL_COLOR_BUFFER_BIT);

	//Bind program
	glUseProgram(*window->getgProgramID());

	//Enable vertex position
	glEnableVertexAttribArray(*window->getgVertexPos2DLocation());

	//Set vertex data
	glBindBuffer(GL_ARRAY_BUFFER, *window->getgVBO());
	glVertexAttribPointer(*window->getgVertexPos2DLocation(), 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), NULL);

	//Set index data and render
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *window->getgIBO());
	glDrawElements(GL_TRIANGLE_FAN, 4, GL_UNSIGNED_INT, NULL);

	//Disable vertex position
	glDisableVertexAttribArray(*window->getgVertexPos2DLocation());

	//Unbind program
	glUseProgram(NULL);
}