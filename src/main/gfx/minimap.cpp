#include "game_window.h"
#include "minimap.h"
#include "../parser_yaml/graphics_parser.h"

MiniMap::MiniMap(GameWindow& owner, GraphicsParser& graphicsParser) :
	owner(owner),
	size(graphicsParser.getPantalla().minimapa_ancho, graphicsParser.getPantalla().hud_alto),
	offset(owner.ancho_pantalla - graphicsParser.getPantalla().minimapa_ancho, owner.alto_pantalla - graphicsParser.getPantalla().hud_alto)
{
	double d = owner.player.board.sizeX + owner.player.board.sizeY;
	scale = r2(size.x / (d + 2), size.y / (d + 1));
}

MiniMap::~MiniMap() {}

void MiniMap::draw() {
	//Dibujo fondo
	SDL_Rect destinoFondoMinimapa = { (int)offset.x, (int)offset.y, (int)size.x, (int)size.y };
	SDL_SetRenderDrawColor(owner.getRenderer(), 15, 15, 15, 255);
	SDL_RenderFillRect(owner.getRenderer(), &destinoFondoMinimapa);
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
			SDL_SetRenderDrawColor(owner.getRenderer(), color.r, color.g, color.b, 255);
			SDL_Rect terreno = { p.x, p.y, ts.x, ts.y };
			SDL_RenderFillRect(owner.getRenderer(), &terreno);
		}
	}
	SDL_Point es = {(int)ceil((scale.x + scale.y) / 2), (int)ceil((scale.x + scale.y) / 2)};
	SDL_Point d = {(ts.x - es.x)/2, (ts.y - es.y)/2};
	auto& board = owner.player.board;
	board.mapEntities([this, es, d](shared_ptr<Entity> e) {
		if (e) {
			if (this->owner.player.getVisibility(*e) != INVISIBLE) {
				SDL_Color color = this->owner.getColor(e->owner.getId());
				auto p = this->boardToScreenPosition(e->center()-r2(.5,.5));
				p.x += d.x; p.y += d.y;
				for (auto se : owner.sController->getSelection()) {
					if (se == e) {
						color = { 255,255,255 };
						break;
					}	
				}
				SDL_SetRenderDrawColor(this->owner.getRenderer(), color.r, color.g, color.b, 255);
				SDL_Rect entidad = { p.x, p.y, es.x, es.y };
				SDL_RenderFillRect(this->owner.getRenderer(), &entidad);
			}
		}
	});
	auto pul = boardToScreenPosition(owner.isoview->screenToBoardPosition({ 0,0 }));
	auto pbr = boardToScreenPosition(owner.isoview->screenToBoardPosition(owner.isoview->getSize()));
	SDL_Point s = {pbr.x - pul.x, pbr.y - pul.y};
	SDL_SetRenderDrawColor(owner.getRenderer(), 255, 255, 255, 255);
	SDL_Rect target = {pul.x, pul.y, s.x, s.y};
	SDL_RenderDrawRect(owner.getRenderer(), &target);
}

SDL_Color MiniMap::tmpGetColor(std::string name) {
	if (name == "Agua")
		return{ 0, 0, 127 };
	if (name == "Piedra")
		return{ 127, 127, 127 };

	return{ 0, 127, 0 };
}

SDL_Point MiniMap::boardToScreenPosition(r2 p) {
	return {(int)(offset.x + (p.x - p.y + owner.player.board.sizeY) * scale.x), (int)(offset.y + (p.x + p.y) * scale.y)};
}

