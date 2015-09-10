#ifndef __GFX_GAMEWINDOW_H__

#include <SDL2/SDL.h>
#include "../game.h"
#include "../log/logger.h"
#include "../defines/defines.h"
#include "../controllers/event_handler.h"
#include "sprite_sheet.h"

class GameWindow {
protected:
	Game* model; //= NULL;
	SDL_Window* window; //= NULL;
	SDL_Renderer* renderer;// = NULL;
	std::map<std::string, SpriteSheet*> spritesSheets;
	bool exitGame;
	static bool sdlInitialized; // = false
	static bool initialize();
	bool endOfGame();	
	void processInput();
	void update();
	void render();

public:
	GameWindow(Game* game);
	~GameWindow();
	int start();
	void init();
};
#endif // __GFX_GAMEWINDOW_H__
