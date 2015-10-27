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
				SDL_Rect terreno = { (int)offset.x + (p.x - p.y + owner.player.board.sizeY) * scale.x, (int)offset.y + (p.x + p.y) * scale.y, ceil(2 * scale.x), ceil(scale.y) };
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
			SDL_Rect entidad = { offset.x + (p.x - p.y + owner.player.board.sizeY) * scale.x, offset.y + (p.x + p.y) * scale.y, ceil((scale.x + scale.y) / 2), ceil((scale.x + scale.y) / 2) };
			SDL_RenderFillRect(renderer, &entidad);
		}
	}
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	//SDL_Point p1 = boardToScreenPosition(owner.isoview->screenToBoardPosition({ 0,0 }));
	//SDL_Point p2 = boardToScreenPosition(owner.isoview->screenToBoardPosition({ 0,3 * owner.alto_pantalla / 4 }));
	//SDL_Point p3 = boardToScreenPosition(owner.isoview->screenToBoardPosition({owner.ancho_pantalla,3 * owner.alto_pantalla / 4 }));
	//SDL_Point p4 = boardToScreenPosition(owner.isoview->screenToBoardPosition({owner.ancho_pantalla,0 }));
	//SDL_Point p5 = boardToScreenPosition(owner.isoview->screenToBoardPosition({ 0,0 }));
	//p1.x = p1.x + offset.x;
	//p1.y = p1.y + offset.y;
	//p2.x = p2.x + offset.x;
	//p2.y = p2.y + offset.y;
	//p3.x = p3.x + offset.x;
	//p3.y = p3.y + offset.y;
	//p4.y = p4.y + offset.y;
	//p4.x = p4.x + offset.x;
	//p5.y = p5.y + offset.y;
	//p5.x = p5.x + offset.x;
	//SDL_Point points[] = { p1,p2,p3,p4,p5 };
	//SDL_RenderDrawLines(renderer, points, 5);
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

SDL_Point MiniMap::boardToScreenPosition(r2 boardPos) {
	boardPos -= owner.getFocus();
	SDL_Point ret = {
		(int)((boardPos.x - boardPos.y) * TILE_WIDTH_DEFAULT / 2) + (size.x) / 2,
		(int)((boardPos.x + boardPos.y) * TILE_HEIGHT_DEFAULT / 2) + (size.y - TILE_HEIGHT_DEFAULT) / 2 };
	return ret;
}