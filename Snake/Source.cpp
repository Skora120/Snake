#include <string>
#include <iostream>
#include <math.h>
#include <time.h>
// OpenGL / glew Headers
#define GL3_PROTOTYPES 1
#include <GL/glew.h>

// SDL2 Headers
#include <SDL.h>

// Game Objects Headers
#include "Snake.h"
#include "Apple.h"


std::string programName = "Snake";

// Our SDL_Window ( just like with SDL2 wihout OpenGL)
SDL_Window *mainWindow;

SDL_Renderer* renderer;

// Our opengl context handle
SDL_GLContext mainContext;


unsigned int windowWidth;
unsigned int windowHeight;

unsigned int score;
unsigned int highestScore;

// Snake Game Functions
void drawApple(Apple *apple);
void drawSnake(Snake *snake);

bool isCollisionSnakeSnake(Snake *snake);
bool isCollisionSnakeWall(Snake *snake);
bool isCollisionSnakeApple(Snake *snake, Apple *apple);

void setAppleCoords(Snake *snake, Apple *apple);
void checkForCollsions(Snake *snake, Apple *apple);

bool SetOpenGLAttributes();
void PrintSDL_GL_Attributes();
void CheckSDLError(int line);
void RunGame();
void Cleanup();


bool Init()
{
	windowWidth = 360;
	windowHeight = 360;

	srand(time(NULL));
	// Initialize SDL's Video subsystem
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		std::cout << "Failed to init SDL\n";
		return false;
	}

	// Create window
	mainWindow = SDL_CreateWindow(programName.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		windowWidth, windowHeight, SDL_WINDOW_OPENGL);


	auto context = SDL_GL_CreateContext(mainWindow);

	// Check that everything worked out okay
	if (!mainWindow)
	{
		std::cout << "Unable to create window\n";
		CheckSDLError(__LINE__);
		return false;
	}

	// Create our opengl context and attach it to our window
	mainContext = SDL_GL_CreateContext(mainWindow);

	SetOpenGLAttributes();

	// This makes our buffer swap syncronized with the monitor's vertical refresh
	SDL_GL_SetSwapInterval(1);

	return true;
}

void drawApple(Apple * apple)
{
	SDL_Rect r;
	for (int i = 0; i < 4; i++) {
		r.x = apple->getApplePart(i).x + apple->getX();
		r.y = apple->getApplePart(i).y + apple->getY();
		r.w = apple->getSizeWidth();
		r.h = apple->getSizeHeight();

		SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
		SDL_RenderFillRect(renderer, &r);
	}
}

void drawSnake(Snake *snake)
{
	int size = snake->getSize();
	double width = snake->getWidth();
	double height = snake->getHeight();

	SDL_Rect r;
	for (int i = 0; i < size; i++) {
		r.x = snake->getSnakePart(i).x;
		r.y = snake->getSnakePart(i).y;
		r.w = width;
		r.h = height;

		SDL_SetRenderDrawColor(renderer, snake->red, snake->green, snake->blue, 255);
		SDL_RenderFillRect(renderer, &r);


		// Draw head
		if (i == 0) {
			r.x = snake->getSnakePart(i).x;
			r.y = snake->getSnakePart(i).y;
			r.w = width / 3;
			r.h = height / 3;
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

			SDL_RenderFillRect(renderer, &r);

			// Eyes
			if (snake->getCurrentDirection() == Snake::Up) {
				r.x = snake->getSnakePart(i).x + 15;
				r.y = snake->getSnakePart(i).y;
				r.w = width / 3;
				r.h = height / 3;

				SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
				SDL_RenderFillRect(renderer, &r);
			}
			else if (snake->getCurrentDirection() == Snake::Down) {
				r.x = snake->getSnakePart(i).x + 15;
				r.y = snake->getSnakePart(i).y;
				r.w = width / 3;
				r.h = height / 3;


				SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
				SDL_RenderFillRect(renderer, &r);
			}

			// Mouth
			if (snake->getCurrentDirection() == Snake::Down) {
				r.x = snake->getSnakePart(i).x + 5;
				r.y = snake->getSnakePart(i).y + 15;
				r.w = width / 2;
				r.h = height / 4;

				SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
			}
			else if (snake->getCurrentDirection() == Snake::Left) {
				r.x = snake->getSnakePart(i).x + 0;
				r.y = snake->getSnakePart(i).y + 15;
				r.w = width / 2;
				r.h = height / 4;

				SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
			}
			else if (snake->getCurrentDirection() == Snake::Right) {
				r.x = snake->getSnakePart(i).x + 10;
				r.y = snake->getSnakePart(i).y + 15;
				r.w = width / 2;
				r.h = height / 4;

				SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
			}
			SDL_RenderFillRect(renderer, &r);
		}
	}
}

bool isCollisionSnakeSnake(Snake * snake)
{
	unsigned int snakeSize = snake->getSize();
	SnakePart head = snake->getSnakePart(0);
	SnakePart *temp;

	double result = 0.0;
	for (int i = 1; i < snakeSize; i++) {
		temp = &snake->getSnakePart(i);
		result = sqrt((temp->x - head.x)*(temp->x - head.x) + (temp->y - head.y)*(temp->y - head.y));
		if (result == 0)
			return true;
	}
	return false;
}

bool isCollisionSnakeWall(Snake * snake)
{
	SnakePart head = snake->getSnakePart(0);

	if (head.x < 0 || head.x >= windowWidth || head.y >= windowHeight || head.y < 0)
		return true;
	return false;
}

bool isCollisionSnakeApple(Snake * snake, Apple * apple)
{
	unsigned int snakeSize = snake->getSize();
	SnakePart head = snake->getSnakePart(0);

	if (head.x == apple->getX() && head.y == apple->getY())
		return true;
	return false;
}

void setAppleCoords(Snake * snake, Apple * apple)
{
	unsigned apple_width = 0;
	unsigned apple_height = 0;

	bool is_valid = false;
	while (!is_valid) {
		apple_width = rand() % windowWidth / 10;
		apple_height = rand() % windowHeight / 10;

		if (apple_width % 2 != 0) {
			apple_width = rand() % windowWidth / 10;
		}
		if (apple_height % 2 != 0) {
			apple_height = rand() % windowHeight / 10;
		}

		if (apple_height % 2 == 0 && apple_width % 2 == 0) {
			is_valid = true;
			apple->setX(apple_width * 10);
			apple->setY(apple_height * 10);
		}
	}
}

void checkForCollsions(Snake * snake, Apple * apple)
{
	std::string str = "Snake      Score: ";
	const char *cstr;

	if (isCollisionSnakeSnake(snake) || isCollisionSnakeWall(snake)) {
		snake->reset(windowWidth / 2, windowHeight / 2);
		score = 0;
		str += "0";
		str += " Highest score: " + std::to_string(highestScore);
		cstr = str.c_str();
		SDL_SetWindowTitle(mainWindow, cstr);
	}
	else if (isCollisionSnakeApple(snake, apple)) {
		++score;
		if (highestScore < score) {
			highestScore = score;
		}

		(*snake)++;
		drawSnake(snake);
		setAppleCoords(snake, apple);
		str += std::to_string(score);
		str += " Highest score: " + std::to_string(highestScore);
		cstr = str.c_str();
		SDL_SetWindowTitle(mainWindow, cstr);

	}

}

bool SetOpenGLAttributes()
{
	// Set our OpenGL version.
	// SDL_GL_CONTEXT_CORE gives us only the newer version, deprecated functions are disabled
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	// 3.2 is part of the modern versions of OpenGL, but most video cards whould be able to run it
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);

	// Turn on double buffering with a 24bit Z buffer.
	// You may need to change this to 16 or 32 for your system
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	return true;
}

int main(int argc, char *argv[])
{
	if (!Init())
		return -1;
	renderer = SDL_CreateRenderer(mainWindow, -1, SDL_RENDERER_ACCELERATED);

	SDL_GL_SwapWindow(mainWindow);

	RunGame();
	Cleanup();

	return 0;
}

void RunGame()
{
	bool loop = true;
	unsigned int lastTime = 0, currentTime;

	Snake *snake = new Snake(windowWidth / 2, windowHeight / 2, 20, 20);
	Apple *apple = new Apple(300, 300);
	setAppleCoords(snake, apple);

	(*snake)++;
	(*snake)++;

	bool moved = false;
	bool epilepsyMode = false;

	while (loop)
	{
		checkForCollsions(snake, apple);

		SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255);
		SDL_RenderClear(renderer);
		SDL_Event event;


		currentTime = SDL_GetTicks();
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
				loop = false;

			if (event.type == SDL_KEYDOWN)
			{
				switch (event.key.keysym.sym)
				{
				case SDLK_ESCAPE:
					loop = false;
					break;
				case SDLK_RIGHT:
				case SDLK_d:
					if (snake->getCurrentDirection() != Snake::Left || snake->getSize() == 1) {
						snake->snakeMoveRight();
					}
					break;
				case SDLK_DOWN:
				case SDLK_s:
					if (snake->getCurrentDirection() != Snake::Up || snake->getSize() == 1) {
						snake->snakeMoveDOWN();
					}
					break;
				case SDLK_UP:
				case SDLK_w:
					if (snake->getCurrentDirection() != Snake::Down || snake->getSize() == 1) {
						snake->snakeMoveUP();
					}
					break;
				case SDLK_LEFT:
				case SDLK_a:
					if (snake->getCurrentDirection() != Snake::Right || snake->getSize() == 1) {
						snake->snakeMoveLeft();
					}
					break;
				case SDLK_SPACE:
					snake->snakeMoveCurrent();
					break;
				case SDLK_k:
					(*snake)++;
					snake->snakeMoveCurrent();
					drawSnake(snake);
					break;
				case SDLK_j:
					if (epilepsyMode) {
						epilepsyMode = false;
					}
					else {
						epilepsyMode = true;
					}

					break;
				default:
					break;
				}

				moved = true;
				snake->snakeMoveCurrent();
			}
		}


		if (currentTime > lastTime + 500) {
			snake->snakeMoveCurrent();
			lastTime = currentTime;
			moved = false;

			if (score % 3 == 0 && score != 0) {
				snake->red = rand() % 255;
				snake->green = rand() % 255;
				snake->blue = rand() % 255;
			}
		}

		if (epilepsyMode) {
			snake->red = rand() % 255;
			snake->green = rand() % 255;
			snake->blue = rand() % 255;
		}

		drawApple(apple);
		drawSnake(snake);

		SDL_RenderPresent(renderer);
	}
}

void Cleanup()
{
	// Delete our OpengL context
	SDL_GL_DeleteContext(mainContext);

	// Destroy our window
	SDL_DestroyWindow(mainWindow);

	// Shutdown SDL 2
	SDL_Quit();
}

void CheckSDLError(int line = -1)
{
	std::string error = SDL_GetError();

	if (error != "")
	{
		std::cout << "SLD Error : " << error << std::endl;

		if (line != -1)
			std::cout << "\nLine : " << line << std::endl;

		SDL_ClearError();
	}
}

void PrintSDL_GL_Attributes()
{
	int value = 0;
	SDL_GL_GetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, &value);
	std::cout << "SDL_GL_CONTEXT_MAJOR_VERSION : " << value << std::endl;

	SDL_GL_GetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, &value);
	std::cout << "SDL_GL_CONTEXT_MINOR_VERSION: " << value << std::endl;
}