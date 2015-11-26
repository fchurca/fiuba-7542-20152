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
	SDL_Point centro = owner.boardToScreenPosition(entity.center());
	SDL_SetRenderDrawColor(owner.owner.getRenderer(), 255, 255, 255, 255);
	SDL_Rect lineaFondo = { (int)(centro.x - TILE_WIDTH_DEFAULT / 4), (int)(centro.y + 2), TILE_WIDTH_DEFAULT / 2, 2 };
	SDL_RenderFillRect(owner.owner.getRenderer(), &lineaFondo);
	double porcentaje = entity.health.get() / entity.health.max;
	SDL_SetRenderDrawColor(owner.owner.getRenderer(), 255, 0, 0, 255);
	SDL_Rect lineaSalud = { (int)((centro.x - TILE_WIDTH_DEFAULT / 4) * porcentaje), (int)(centro.y + 2), TILE_WIDTH_DEFAULT / 2, 2 };
	SDL_RenderFillRect(owner.owner.getRenderer(), &lineaSalud);
}

void HealthBar::visit(Building& entity) {
	visit((Entity&)entity);
	SDL_Point centro = owner.boardToScreenPosition(entity.center());
	SDL_SetRenderDrawColor(owner.owner.getRenderer(), 255, 255, 255, 255);
	SDL_Rect lineaFondo = { (int)(centro.x - TILE_WIDTH_DEFAULT / 4), (int)(centro.y + 2), TILE_WIDTH_DEFAULT / 2, 2 };
	SDL_RenderFillRect(owner.owner.getRenderer(), &lineaFondo);
	double porcentaje = entity.health.get() / entity.health.max;
	SDL_SetRenderDrawColor(owner.owner.getRenderer(), 255, 0, 0, 255);
	SDL_Rect lineaSalud = { (int)((centro.x - TILE_WIDTH_DEFAULT / 4) * porcentaje), (int)(centro.y + 2), TILE_WIDTH_DEFAULT / 2, 2 };
	SDL_RenderFillRect(owner.owner.getRenderer(), &lineaSalud);
}

void HealthBar::visit(UnfinishedBuilding& entity) {
	visit((Building&)entity);
}

void HealthBar::visit(Flag& entity) {
	visit((Building&)entity);
	SDL_Point centro = owner.boardToScreenPosition(entity.center());
	SDL_SetRenderDrawColor(owner.owner.getRenderer(), 255, 255, 255, 255);
	SDL_Rect lineaFondo = { (int)(centro.x - TILE_WIDTH_DEFAULT / 4), (int)(centro.y + 2), TILE_WIDTH_DEFAULT / 2, 2 };
	SDL_RenderFillRect(owner.owner.getRenderer(), &lineaFondo);
	double porcentaje = entity.health.get() / entity.health.max;
	SDL_SetRenderDrawColor(owner.owner.getRenderer(), 255, 0, 0, 255);
	SDL_Rect lineaSalud = { (int)((centro.x - TILE_WIDTH_DEFAULT / 4) * porcentaje), (int)(centro.y + 2), TILE_WIDTH_DEFAULT / 2, 2 };
	SDL_RenderFillRect(owner.owner.getRenderer(), &lineaSalud);
}

