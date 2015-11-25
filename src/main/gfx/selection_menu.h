#ifndef __GFX_SELECTION_MENU_H__
#define __GFX_SELECTION_MENU_H__
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "../model/geometry.h"
class GameWindow;
class GraphicsParser;

class SelectionMenu : public EntityVisitor {
private:
	GameWindow& owner;
	r2 size;
	r2 offset;
	std::string outText;
protected:
public:
	void draw();
	SelectionMenu(GameWindow& owner, GraphicsParser& graphicsParser);
	~SelectionMenu();
	virtual void visit(Entity& e);
	virtual void visit(Unit& e);
	virtual void visit(Worker& e);
	virtual void visit(Resource& e);
	virtual void visit(Building& e);
	virtual void visit(UnfinishedBuilding& e);
};
#endif // __GFX_SELECTION_MENU_H__
