#include "isoview.h"
#include "game_window.h"
#include "health_bar.h"
using namespace std;

HealthBar::HealthBar(IsoView& owner) :
	owner(owner)
{
}

HealthBar::~HealthBar() {
}

void HealthBar::visit(Entity& entity) {
	SDL_Color color = owner.owner.getColor(entity.owner.getId());
	SDL_SetRenderDrawColor(owner.owner.getRenderer(), color.r, color.g, color.b, 255);
	SDL_Point centro = owner.boardToScreenPosition(entity.center());
	SDL_Rect linea = { (int)(centro.x - TILE_WIDTH_DEFAULT / 4), (int)(centro.y), TILE_WIDTH_DEFAULT / 2, 2 };
	SDL_RenderFillRect(owner.owner.getRenderer(), &linea);
}
void HealthBar::visit(Unit& entity) {
	visit((Entity&)entity);
}