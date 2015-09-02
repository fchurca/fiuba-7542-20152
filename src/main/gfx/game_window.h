#ifndef __GFX_GAMEWINDOW_H__

#include <SDL2/SDL.h>

class GameWindow {
protected:
	SDL_Window* window = NULL;
	static bool sdlInitialized; // = false
	static bool initialize();
public:
	GameWindow();
	~GameWindow();
};
#endif // __GFX_GAMEWINDOW_H__