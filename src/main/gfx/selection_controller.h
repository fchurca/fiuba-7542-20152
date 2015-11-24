#ifndef __GFX_SELECTION_CONTROLLER_H__
#define __GFX_SELECTION_CONTROLLER_H__
#include <string>
#include "../model/entity.h"
class GameWindow;

class SelectionController : public EntityVisitor{
private:
	GameWindow& owner;
	std::vector<std::shared_ptr<Entity>> selection;
protected:
public:
	SelectionController(GameWindow& owner);
	~SelectionController();
	virtual void visit(Entity& e);
	virtual void visit(Unit& e);
	void setSelection();
	std::vector<std::shared_ptr<Entity>> getSelection();
};
#endif // __GFX_SELECTION_CONTROLLER_H__