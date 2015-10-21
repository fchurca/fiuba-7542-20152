#include "game_window.h"
#include "minimap.h"

MiniMap::MiniMap(GameWindow& owner) :
	owner(owner),
	size(owner.ancho_pantalla / 4, owner.alto_pantalla / 4),
	offset(3 * owner.ancho_pantalla / 4, 3 * owner.alto_pantalla / 4)
{
	double d = owner.player.board.sizeX + owner.player.board.sizeY;
	scale = r2(size.x / (d + 2), size.y / (d + 1));
}

MiniMap::~MiniMap() {}

void MiniMap::drawMinimap(SDL_Renderer* renderer) {
	////Minimapa
	for (int i = 0; i < owner.player.board.sizeX; i++) {
		for (int j = 0; j < owner.player.board.sizeY; j++) {
			std::shared_ptr<Entity> t = owner.player.board.getTerrain(i, j);
			if (owner.player.getVisibility(*t) != INVISIBLE) {
				SDL_Color color = tmpGetColor(t->name);
				auto p = t->getPosition();
				SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 255);
				SDL_Rect terreno = { offset.x + (p.x - p.y + owner.player.board.sizeY) * scale.x, offset.y + (p.x + p.y) * scale.y, ceil(2 * scale.x), ceil(scale.y) };
				SDL_RenderFillRect(renderer, &terreno);
			}
		}
	}
	for (auto e : owner.player.board.getEntities()) {
		if (owner.player.getVisibility(*e) != INVISIBLE) {
			SDL_Color color = getColor(e->owner.getId());
			auto p = e->getPosition();
			if (owner.getSelection() == e)
				color = { 255,255,255 };
			SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 255);
			SDL_Rect entidad = { offset.x + (p.x - p.y + owner.player.board.sizeY) * scale.x, offset.y + (p.x + p.y) * scale.y, ceil(2 * scale.x), ceil(scale.y) };
			SDL_RenderFillRect(renderer, &entidad);
		}
	}
	//r2 vsi = owner.screenToBoardPosition({0, 0});
	//r2 vid = owner.screenToBoardPosition({ 3 * owner.ancho_pantalla / 4, 3 * owner.alto_pantalla / 4 });
	//SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

}

SDL_Color MiniMap::tmpGetColor(std::string name) {
	if (name == "agua")
		return{ 0, 0, 127 };
	if (name == "piedra")
		return{ 127, 127, 127 };

	return{ 0, 127, 0 };
}


SDL_Color MiniMap::getColor(int id) {
	Uint8 r = (id & 2) * 255;
	Uint8 g = (id & 1) * 255;
	Uint8 b = (id & 4) * 255;
	return{ r, g, b };
}