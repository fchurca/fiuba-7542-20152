#ifndef __GFX_GAMEWINDOW_H__

#include <SDL2/SDL.h>
#include "../log/logger.h"
#include "sprite_sheet.h"
#include "../defines/defines.h"

class GameWindow {
protected:
	SDL_Window* window; //= NULL;
	SDL_Renderer* renderer;// = NULL;
	static bool sdlInitialized; // = false
	static bool initialize();
public:
	GameWindow();
	~GameWindow();
};
#endif // __GFX_GAMEWINDOW_H__
