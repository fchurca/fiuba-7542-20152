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
	SDL_Point ts = {(int)ceil(2 * scale.x), (int)ceil(scale.y)};
	for (int i = 0; i < owner.player.board.sizeX; i++) {
		for (int j = 0; j < owner.player.board.sizeY; j++) {
			std::shared_ptr<Entity> t = owner.player.board.getTerrain(i, j);
			SDL_Color color = { 0, 0, 0 };
			if (owner.player.getVisibility(*t) != INVISIBLE) {
				color = tmpGetColor(t->name);
			}
			auto p = boardToScreenPosition(t->getPosition());
			SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 255);
			SDL_Rect terreno = { p.x, p.y, ts.x, ts.y };
			SDL_RenderFillRect(renderer, &terreno);
		}
	}
	SDL_Point es = {(int)ceil((scale.x + scale.y) / 2), (int)ceil((scale.x + scale.y) / 2)};
	SDL_Point d = {(ts.x - es.x)/2, (ts.y - es.y)/2};
	for (auto e : owner.player.board.getEntities()) {
		if (owner.player.getVisibility(*e) != INVISIBLE) {
			SDL_Color color = getColor(e->owner.getId());
			auto p = boardToScreenPosition(e->center()-r2(.5,.5));
			p.x += d.x; p.y += d.y;
			if (owner.getSelection() == e)
				color = { 255,255,255 };
			SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 255);
			SDL_Rect entidad = { p.x, p.y, es.x, es.y };
			SDL_RenderFillRect(renderer, &entidad);
		}
	}
	auto pul = boardToScreenPosition(owner.isoview->screenToBoardPosition({ 0,0 }));
	auto pbr = boardToScreenPosition(owner.isoview->screenToBoardPosition(owner.isoview->getSize()));
	SDL_Point s = {pbr.x - pul.x, pbr.y - pul.y};
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_Rect target = {pul.x, pul.y, s.x, s.y};
	SDL_RenderDrawRect(renderer, &target);
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

SDL_Point MiniMap::boardToScreenPosition(r2 p) {
	return {(int)(offset.x + (p.x - p.y + owner.player.board.sizeY) * scale.x), (int)(offset.y + (p.x + p.y) * scale.y)};
}

