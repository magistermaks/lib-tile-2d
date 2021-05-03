
#include "window.hpp"
#include <string>
#include <fstream>

Window* _window;
GLuint gProgramID = 0;

SDL_GLContext gContext;
GLint gVertexPos2DLocation = -1;
GLuint gVBO = 0;
GLuint gIBO = 0;

double time = 0;

class Engine {
private:

	static void printProgramLog(GLuint program) {
		//Make sure name is shader
		if (glIsProgram(program)) {
			//Program log length
			int infoLogLength = 0;
			int maxLength = infoLogLength;

			//Get info string length
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			//Allocate string
			char* infoLog = new char[maxLength];

			//Get info log
			glGetProgramInfoLog(program, maxLength, &infoLogLength, infoLog);
			if (infoLogLength > 0) {
				//Print Log
				std::cout << infoLog << '\n';
			}

			//Deallocate string
			delete[] infoLog;
		}
		else {
			std::cout << "Name " << program << " is not a program\n";
		}
	}

	static void printShaderLog(GLuint shader) {
		//Make sure name is shader
		if (glIsShader(shader)) {
			//Shader log length
			int infoLogLength = 0;
			int maxLength = infoLogLength;

			//Get info string length
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

			//Allocate string
			char* infoLog = new char[maxLength];

			//Get info log
			glGetShaderInfoLog(shader, maxLength, &infoLogLength, infoLog);
			if (infoLogLength > 0) {
				//Print Log
				std::cout << infoLog << '\n';
			}

			//Deallocate string
			delete[] infoLog;
		}
		else {
			std::cout << "Name " << shader << " is not a shader\n";
		}
	}

	static std::string loadShader(std::string fname) {
		std::ifstream file(fname);
		if (file.is_open()) {
			std::string content((std::istreambuf_iterator<char>(file)),
				std::istreambuf_iterator<char>());
			file.close();
			return content;
		}
		std::cout << "File not found!\n";
		return "";
	}

	static bool initGL() {
		bool success = true;

		//Generate program
		gProgramID = glCreateProgram();

		GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

		//Get vertex source
		std::string vShader = loadShader(".\\src\\shader\\vShader.vert");
		if (vShader.length() < 2) return false;
		auto vstr = vShader.c_str();
		const GLchar* vertexShaderSource[] = { vstr };

		//Set vertex source
		glShaderSource(vertexShader, 1, vertexShaderSource, NULL);

		//Compile vertex source
		glCompileShader(vertexShader);

		//Check vertex shader for errors
		GLint vShaderCompiled = GL_FALSE;
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &vShaderCompiled);
		if (vShaderCompiled != GL_TRUE) {
			std::cout << "Unable to compile vertex shader " << vertexShader << '\n';
			printShaderLog(vertexShader);
			success = false;
		}
		else {
			//Attach vertex shader to program
			glAttachShader(gProgramID, vertexShader);


			//Create fragment shader
			GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

			//Get fragment source
			std::string fShader = loadShader(".\\src\\shader\\fShader.frag");
			if (fShader.length() < 2) return false;
			auto fstr = fShader.c_str();
			const GLchar* fragmentShaderSource[] = { fstr };

			//Set fragment source
			glShaderSource(fragmentShader, 1, fragmentShaderSource, NULL);

			//Compile fragment source
			glCompileShader(fragmentShader);

			//Check fragment shader for errors
			GLint fShaderCompiled = GL_FALSE;
			glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &fShaderCompiled);
			if (fShaderCompiled != GL_TRUE) {
				std::cout << "Unable to compile fragment shader " << fragmentShader << '\n';
				printShaderLog(fragmentShader);
				success = false;
			}
			else {
				//Attach fragment shader to program
				glAttachShader(gProgramID, fragmentShader);


				//Link program
				glLinkProgram(gProgramID);

				//Check for errors
				GLint programSuccess = GL_TRUE;
				glGetProgramiv(gProgramID, GL_LINK_STATUS, &programSuccess);
				if (programSuccess != GL_TRUE) {
					std::cout << "Error linking program " << gProgramID;
					printProgramLog(gProgramID);
					success = false;
				}
				else {
					//Get vertex attribute location
					gVertexPos2DLocation = glGetAttribLocation(gProgramID, "LVertexPos2D");
					if (gVertexPos2DLocation == -1) {
						std::cout << "LVertexPos2D is not a valid glsl program variable!\n";
						success = false;
					}
					else {
						//Initialize clear color
						glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

						GLfloat vertexData[] =
						{
							-0.6f, -0.5f,//left bottom
							 0.6f, -0.5f,//right bottom
							 0.5f,  0.5f,//right up
							-0.5f,  0.5f//left up
						};

						//IBO data
						GLuint indexData[] =
						{
							0, 1, 2,
							0, 2, 3
						};

						//Create VBO
						glGenBuffers(1, &gVBO);
						glBindBuffer(GL_ARRAY_BUFFER, gVBO);
						glBufferData(GL_ARRAY_BUFFER, 2 * 4 * sizeof(GLfloat), vertexData, GL_STATIC_DRAW);

						//Create IBO
						glGenBuffers(1, &gIBO);
						glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gIBO);
						glBufferData(GL_ELEMENT_ARRAY_BUFFER, 2 * 3 * sizeof(GLuint), indexData, GL_STATIC_DRAW);
					}
				}
			}
		}

		return success;
	}

public:

	/// Initialize LibTile2D
	static bool lib_init() {

		bool success = true;

		//Initialize SDL
		if (SDL_Init(SDL_INIT_VIDEO) < 0) {
			std::cout << "SDL could not initialize! SDL Error: " << SDL_GetError() << '\n';
			success = false;
		}

		return success;
	}

	static bool lib_initGL(Window* window) {
		bool success = true;

		_window = window;
		//Use OpenGL 3.1 core
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

		if (window->getHandle() == NULL) {
			success = false;
		}
		else {
			//Create context
			gContext = SDL_GL_CreateContext(window->getHandle());
			if (gContext == NULL) {
				std::cout << "OpenGL context could not be created! SDL Error: " << SDL_GetError() << '\n';
				success = false;
			}
			else {
				//Initialize GLEW
				glewExperimental = GL_TRUE;
				GLenum glewError = glewInit();
				if (glewError != GLEW_OK) {
					std::cout << "Error initializing GLEW! " << glewGetErrorString(glewError) << '\n';
				}

				//Use Vsync
				if (SDL_GL_SetSwapInterval(1) < 0) {
					std::cout << "Warning: Unable to set VSync! SDL Error: " << SDL_GetError() << '\n';
				}

				//Initialize OpenGL
				if (!initGL()) {
					std::cout << "Unable to initialize OpenGL!\n";
					success = false;
				}
			}
		}
		return success;
	}

	/// Exit LibTile2D
	static void lib_exit() {
		//Deallocate program
		glDeleteProgram(gProgramID);

		SDL_Quit();
	}

	static void render() {
		//Clear color buffer
		glClear(GL_COLOR_BUFFER_BIT);

		//Bind program
		glUseProgram(gProgramID);

		//Enable vertex position
		glEnableVertexAttribArray(gVertexPos2DLocation);

		time += 0.01;
		//Set vertex data
		glBindBuffer(GL_ARRAY_BUFFER, gVBO);
		//GLfloat* ptr = (GLfloat*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
		//ptr[0] = sin(time) * 0.6;
		//glUnmapBuffer(GL_ARRAY_BUFFER);
		glVertexAttribPointer(gVertexPos2DLocation, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), NULL);

		//Set index data and render
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gIBO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);

		//Disable vertex position
		glDisableVertexAttribArray(gVertexPos2DLocation);

		//Unbind program
		glUseProgram(NULL);
	}
};
