#ifndef __GFX_MENU_H__
#define __GFX_MENU_H__
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "../model/geometry.h"
class GameWindow;

class Menu {
private:
	GameWindow& owner;
	TTF_Font* font;
	r2 size;
	r2 offset;
protected:
public:
	void draw(SDL_Renderer* renderer);
	std::string completeLine(std::string line, TTF_Font* font);
	Menu(GameWindow& owner);
	~Menu();
};
#endif // __GFX_MENU_H__