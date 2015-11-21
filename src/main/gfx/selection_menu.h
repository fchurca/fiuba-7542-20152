#ifndef __GFX_SELECTION_MENU_H__
#define __GFX_SELECTION_MENU_H__
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "../model/geometry.h"
class GameWindow;
class GraphicsParser;

class SelectionMenu {
private:
	GameWindow& owner;
	r2 size;
	r2 offset;
protected:
public:
	void draw();
	SelectionMenu(GameWindow& owner, GraphicsParser& graphicsParser);
	~SelectionMenu();
};
#endif // __GFX_SELECTION_MENU_H__