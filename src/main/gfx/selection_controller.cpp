#include "game_window.h"
#include "selection_controller.h"

SelectionController::SelectionController(GameWindow& owner) :
	owner(owner)
{
}

SelectionController::~SelectionController() {
	selection.clear();
}

void SelectionController::visit(Entity& e) { //TODO Sacar el isUnit, ver otra forma
	isUnit = false;
}

void SelectionController::visit(Unit& e) {
	isUnit = true;
}

void SelectionController::setSelection(rectangle r) {
	selection.clear();
	if (!owner.isSweeping()) {
		std::shared_ptr<Entity> e = owner.board.findEntity(r);
		if (e && owner.player.getVisibility(*e) != INVISIBLE) {
			selection.push_back(e);
		}	
	}
	else {
		std::vector<std::shared_ptr<Entity>> entities = owner.board.selectEntities(r);
		if (entities.size() < 2) {
			selection = entities;
		}
		else {
			for (auto e : entities) {
				if (e->owner.name == owner.player.name) {
					e->visit(*this);
					if (isUnit && owner.player.getVisibility(*e) != INVISIBLE) {
						selection.push_back(e);
					}
				}		
			}
		}
	}
}

void SelectionController::setSelection(std::shared_ptr<Entity> e) {
	selection.clear();
	selection.push_back(e);
}

std::vector<std::shared_ptr<Entity>> SelectionController::getSelection() {
	return selection;
}

void SelectionController::update() {
	int i = 0;
	std::vector<std::shared_ptr<Entity>> entities = getSelection();
	for (auto e : entities) {
		if (e->getDeletable()) {
			selection.erase(selection.begin() + i);
		}
		else
			i++;
	}
}