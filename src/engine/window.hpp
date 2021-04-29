
#include <SDL.h>
#include <gl\glew.h>
#include <SDL_opengl.h>
#include <iostream>

/// Represents a single window
class Window {

	SDL_Window* handle = NULL;
	GLuint gProgramID = 0;

	//OpenGL context
	SDL_GLContext gContext;

	GLint gVertexPos2DLocation = -1;
	GLuint gVBO = 0;
	GLuint gIBO = 0;

	public:


		Window( const char* title, int width, int height ) {

			//Initialization flag
			bool success = true;

			//Initialize SDL
			if (SDL_Init(SDL_INIT_VIDEO) < 0) {
				std::cout << "SDL could not initialize! SDL Error: " <<  SDL_GetError() << '\n';
				success = false;
			}
			else {
				//Use OpenGL 3.1 core
				SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
				SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
				SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

				//Create window
				handle = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
				if (handle == NULL) {
					std::cout << "Window could not be created! SDL Error: " << SDL_GetError() << '\n';
					success = false;
				}
				else {
					//Create context
					gContext = SDL_GL_CreateContext(handle);
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
			}
		}

		~Window() {
			//Deallocate program
			glDeleteProgram(gProgramID);

			//Destroy window	
			SDL_DestroyWindow(handle);
			handle = NULL;

			//Quit SDL subsystems
			SDL_Quit();
		}

		//Initializes rendering program and clear color
		bool initGL() {
			//Success flag
			bool success = true;

			//Generate program
			gProgramID = glCreateProgram();

			//Create vertex shader
			GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

			//Get vertex source
			const GLchar* vertexShaderSource[] =
			{
				"#version 140\nin vec2 LVertexPos2D; void main() { gl_Position = vec4( LVertexPos2D.x, LVertexPos2D.y, 0, 1 ); }"
			};

			//Set vertex source
			glShaderSource(vertexShader, 1, vertexShaderSource, NULL);

			//Compile vertex source
			glCompileShader(vertexShader);

			//Check vertex shader for errors
			GLint vShaderCompiled = GL_FALSE;
			glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &vShaderCompiled);
			if (vShaderCompiled != GL_TRUE) {
				printf("Unable to compile vertex shader %d!\n", vertexShader);
				printShaderLog(vertexShader);
				success = false;
			}
			else {
				//Attach vertex shader to program
				glAttachShader(gProgramID, vertexShader);


				//Create fragment shader
				GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

				//Get fragment source
				const GLchar* fragmentShaderSource[] =
				{
					"#version 140\nout vec4 LFragment; void main() { LFragment = vec4( gl_FragCoord.x / 400, 1.0, 1.0, 1.0 ); }"
				};

				//Set fragment source
				glShaderSource(fragmentShader, 1, fragmentShaderSource, NULL);

				//Compile fragment source
				glCompileShader(fragmentShader);

				//Check fragment shader for errors
				GLint fShaderCompiled = GL_FALSE;
				glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &fShaderCompiled);
				if (fShaderCompiled != GL_TRUE) {
					printf("Unable to compile fragment shader %d!\n", fragmentShader);
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
						printf("Error linking program %d!\n", gProgramID);
						printProgramLog(gProgramID);
						success = false;
					}
					else {
						//Get vertex attribute location
						gVertexPos2DLocation = glGetAttribLocation(gProgramID, "LVertexPos2D");
						if (gVertexPos2DLocation == -1) {
							printf("LVertexPos2D is not a valid glsl program variable!\n");
							success = false;
						}
						else {
							//Initialize clear color
							glClearColor(0.f, 0.f, 0.f, 1.f);

							//VBO data
							GLfloat vertexData[] =
							{
								-0.6f, -0.5f,
								 0.6f, -0.5f,
								 0.5f,  0.5f,
								-0.5f,  0.5f
							};

							//IBO data
							GLuint indexData[] = { 0, 1, 2, 3 };

							//Create VBO
							glGenBuffers(1, &gVBO);
							glBindBuffer(GL_ARRAY_BUFFER, gVBO);
							glBufferData(GL_ARRAY_BUFFER, 2 * 4 * sizeof(GLfloat), vertexData, GL_STATIC_DRAW);

							//Create IBO
							glGenBuffers(1, &gIBO);
							glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gIBO);
							glBufferData(GL_ELEMENT_ARRAY_BUFFER, 4 * sizeof(GLuint), indexData, GL_STATIC_DRAW);
						}
					}
				}
			}

			return success;
		}

		void printProgramLog(GLuint program) {
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
					printf("%s\n", infoLog);
				}

				//Deallocate string
				delete[] infoLog;
			}
			else {
				printf("Name %d is not a program\n", program);
			}
		}

		void printShaderLog(GLuint shader) {
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
					printf("%s\n", infoLog);
				}

				//Deallocate string
				delete[] infoLog;
			}
			else {
				printf("Name %d is not a shader\n", shader);
			}
		}

		bool isOk() {
			return this->handle != NULL;
		}

		/// Get SDL window object
		SDL_Window* getHandle() {
			return this->handle;
		}

		GLuint* getgProgramID() {
			return &this->gProgramID;
		}

		SDL_GLContext* getgContext() {
			return &this->gContext;
		}

		GLint* getgVertexPos2DLocation() {
			return &this->gVertexPos2DLocation;
		}

		GLuint* getgVBO() {
			return &this->gVBO;
		}

		GLuint* getgIBO() {
			return &this->gIBO;
		}

};
