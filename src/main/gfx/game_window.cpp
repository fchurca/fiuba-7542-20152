#include "game_window.h"

bool GameWindow::sdlInitialized = false;

bool GameWindow::initialize() {
	Logger::getInstance()->writeInformation("Initializing graphics");
	if (GameWindow::sdlInitialized) {
		Logger::getInstance()->writeWarning("SDL already initialized");
	} else {
		atexit(SDL_Quit);
		if( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {
			Logger::getInstance()->writeError("SDL could not initialize!");
			Logger::getInstance()->writeError(SDL_GetError());
			GameWindow::sdlInitialized = false;
		} else {
			GameWindow::sdlInitialized = true;
		}
	}

	return GameWindow::sdlInitialized;
}

GameWindow::GameWindow() {
	Logger::getInstance()->writeInformation("Creating window");
	int SCREEN_WIDTH = 640,
		SCREEN_HEIGHT = 480;
	GameWindow::initialize(); 
	window = SDL_CreateWindow("Trabajo Práctico 7542",
			SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
			SCREEN_WIDTH, SCREEN_HEIGHT,
			SDL_WINDOW_SHOWN);
	
	Logger::getInstance()->writeInformation("Creating renderer");
	renderer = SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED );

	// TODO: IMG_INIT


	
	SDL_RenderPresent( renderer );
}

GameWindow::~GameWindow() {
	Logger::getInstance()->writeInformation("Destroying renderer");
	if (renderer != NULL) {
		SDL_DestroyRenderer(renderer);
		renderer = NULL;
	}
	Logger::getInstance()->writeInformation("Destroying window");
	if (window != NULL) {
		Logger::getInstance()->writeWarning("Window never initialized");
		SDL_DestroyWindow(window);
		window = NULL;
	}
}

