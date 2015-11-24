#include "game_window.h"
#include "selection_controller.h"

SelectionController::SelectionController(GameWindow& owner) :
	owner(owner)
{
}

SelectionController::~SelectionController() {
}

void SelectionController::visit(Entity& e) {
}

void SelectionController::visit(Unit& e) {
}

void SelectionController::setSelection() {
}

std::vector<std::shared_ptr<Entity>> SelectionController::getSelection() {
	return selection;
}