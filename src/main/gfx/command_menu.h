#ifndef __GFX_COMMAND_MENU_H__
#define __GFX_COMMAND_MENU_H__
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "../model/geometry.h"
class GameWindow;
class GraphicsParser;

class CommandMenu {
private:
	GameWindow& owner;
	r2 size;
	r2 offset;
protected:
public:
	void draw();
	CommandMenu(GameWindow& owner, GraphicsParser& graphicsParser);
	~CommandMenu();
};
#endif // __GFX_COMMAND_MENU_H__