#ifndef __GFX_COMMAND_MENU_H__
#define __GFX_COMMAND_MENU_H__
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "../model/geometry.h"
class GameWindow;
class GraphicsParser;

class CommandMenu : public EntityVisitor {
private:
	GameWindow& owner;
	r2 size;
	r2 offset;
	std::string outText;
	std::shared_ptr<Entity> currentSelection;
protected:
public:
	void draw();
	void clear();
	std::string intToString(int i);
	CommandMenu(GameWindow& owner, GraphicsParser& graphicsParser);
	~CommandMenu();
	bool isVisibleWorker;
	bool isVisibleUnit;
	bool isVisibleProducer;
	bool showOptions;
	bool positioning;
	int selectedOption;
	virtual void visit(Entity& e);
	virtual void visit(Unit& e);
	virtual void visit(Worker& e);
	virtual void visit(Building& e);
};
#endif // __GFX_COMMAND_MENU_H__
