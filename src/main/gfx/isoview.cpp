#include "isoview.h"
#include "game_window.h"
#include "../parser_yaml/ruleset_parser.h"

using namespace std;

IsoView::IsoView(GameWindow& owner, RulesetParser& rulesetParser) :
	owner(owner),
	size(owner.ancho_pantalla, 3 * owner.alto_pantalla / 4),
	offset(0, 0)
{
	addSpriteSheet(ENTIDAD_DEFAULT_NOMBRE, ENTIDAD_DEFAULT_IMAGEN, ENTIDAD_DEFAULT_PIXEL_REF_X, ENTIDAD_DEFAULT_PIXEL_REF_Y, ENTIDAD_DEFAULT_ALTO_SPRITE, ENTIDAD_DEFAULT_ANCHO_SPRITE, ENTIDAD_DEFAULT_CANTIDAD_SPRITES, ENTIDAD_DEFAULT_FPS, ENTIDAD_DEFAULT_DELAY);
	addSpriteSheet(TERRENO_DEFAULT_NOMBRE, TERRENO_DEFAULT_IMAGEN, TERRENO_DEFAULT_PIXEL_REF_X, TERRENO_DEFAULT_PIXEL_REF_Y, TERRENO_DEFAULT_ALTO_SPRITE, TERRENO_DEFAULT_ANCHO_SPRITE, TERRENO_DEFAULT_CANTIDAD_SPRITES, TERRENO_DEFAULT_FPS, TERRENO_DEFAULT_DELAY);
	addSpriteSheet(PROTAGONISTA_DEFAULT_NOMBRE, PROTAGONISTA_DEFAULT_IMAGEN, PROTAGONISTA_DEFAULT_PIXEL_REF_X, PROTAGONISTA_DEFAULT_PIXEL_REF_Y, PROTAGONISTA_DEFAULT_ALTO_SPRITE, PROTAGONISTA_DEFAULT_ANCHO_SPRITE, PROTAGONISTA_DEFAULT_CANTIDAD_SPRITES, PROTAGONISTA_DEFAULT_FPS, PROTAGONISTA_DEFAULT_DELAY);
	for (auto& t : rulesetParser.getTiposEntidades()) {
		addSpriteSheet(t.nombre, t.imagen, t.pixel_ref_x, t.pixel_ref_y, t.alto_sprite, t.ancho_sprite, t.cantidad_sprites, t.fps, t.delay);
	}

	for (auto& t : rulesetParser.getTiposTerrenos()) {
		addSpriteSheet(t.nombre, t.imagen, t.pixel_ref_x, t.pixel_ref_y, t.alto_sprite, t.ancho_sprite, t.cantidad_sprites, t.fps, t.delay);
	}

	for (auto& t : rulesetParser.getTiposRecursos()) {
		addSpriteSheet(t.nombre, t.imagen, t.pixel_ref_x, t.pixel_ref_y, t.alto_sprite, t.ancho_sprite, t.cantidad_sprites, t.fps, t.delay);
	}
}

IsoView::~IsoView() {
	spriteSheets.clear();
}

SDL_Point IsoView::getSize() {
	return {(int)size.x, (int)size.y};
}

void IsoView::draw(SDL_Renderer* renderer) {
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);
	// Dibujamos el terreno
	r2 margin(1, 1),
		ul = screenToBoardPosition({ 0, 0 }) - margin, // Upper Left
		ur = screenToBoardPosition({ (int)size.x, 0 }) - margin, // Upper Right
		bl = screenToBoardPosition({ 0, (int)size.y }) + margin, // Bottom Left
		br = screenToBoardPosition({ (int)size.x, (int)size.y }) + margin; // Bottom Right
	double ud = ul.x + ul.y - 2, // Upper diagonal
		bd = bl.x + bl.y + 2, // Bottom diagonal
		ld = ul.x - ul.y - 2, // Left diagonal
		rd = ur.x - ur.y + 2; // Right diagonal
	for (size_t x = (size_t)max(0.0, ul.x),
		maxx = min(((double)owner.player.board.sizeX), br.x);
		x < maxx;
		x++) {
		if (x >= owner.player.board.sizeX) {
			break;
		}
		for (size_t y = (size_t)max(max(max(0.0, ur.y), ud - x), x - rd),
			maxy = min(min(min(((double)owner.player.board.sizeY), bl.y), bd - x), x - ld);
			y < maxy;
			y++) {
			if (y >= owner.player.board.sizeY) {
				break;
			}
			auto tile = owner.player.board.getTerrain(x, y);
			if (tile) {
				if (canDraw(tile)) {
					spriteSheets[tile->name]->render(*tile, renderer);
				}
			}
		}
	}
	// Seleccionamos entidades que se pisan con la pantalla
	auto entities = owner.player.board.selectEntities([this](shared_ptr<Entity> e) {
		return canDraw(e); });
	// Ordenamos las entidades por oclusión
	sort(entities.begin(), entities.end(), [](shared_ptr<Entity> a, shared_ptr<Entity> b) {
		return (a->size.x == a->size.y && b->size.x == b->size.y) ?
			(a->getPosition().x + a->getPosition().y + a->size.x < b->getPosition().x + b->getPosition().y + b->size.x) :
			((a->getPosition().x + a->size.x < b->getPosition().x) || (a->getPosition().y + a->size.y < b->getPosition().y)) &&
			!((b->getPosition().x + b->size.x <= a->getPosition().x) || (b->getPosition().y + b->size.y <= a->getPosition().y));
	});
	for (auto& e : entities) {
		auto it = spriteSheets.find(e->name);
		if (it == spriteSheets.end()) {
			Logger::getInstance()->writeWarning("No existe SpriteSheet para este tipo de entidad" + e->name);
			continue;
		}
		it->second->render(*e, renderer);
		if (e->owner.name != DEFAULT_PLAYER_NAME && owner.player.getVisibility(*e) != INVISIBLE) {
			SDL_Color color = owner.getColor(e->owner.getId());
			SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 255);
			SDL_Point centro = boardToScreenPosition(e->center());
			SDL_Rect linea = { (int)(centro.x - TILE_WIDTH_DEFAULT/4), (int)(centro.y), TILE_WIDTH_DEFAULT/2, 2 };
			SDL_RenderFillRect(renderer, &linea);
		}
	}
	if (owner.getSelection()) {
		Uint8 q = 255;
		SDL_SetRenderDrawColor(renderer, q, q, q, q);
		r2 p = owner.getSelection()->getPosition();
		r2 s = owner.getSelection()->size;
		SDL_Point points[] = {
			boardToScreenPosition(p),
			boardToScreenPosition(p + r2(s.x, 0)),
			boardToScreenPosition(p + s),
			boardToScreenPosition(p + r2(0, s.y)),
			boardToScreenPosition(p) };

		SDL_RenderDrawLines(renderer, points, 5);
	}
}

void IsoView::update() {
	for (auto & kv : spriteSheets){
		kv.second->update();
	}
}

bool IsoView::canDraw(shared_ptr<Entity> e) {
	if (!e) {
		return false;
	}
	if (e->getDeletable()) {
		return false;
	}
	Entity& entity = *e;
	SDL_Rect screenRect = { 0, 0, (int)size.x, (int)size.y };
	auto it = spriteSheets.find(entity.name);
	if (it == spriteSheets.end()) {
		Logger::getInstance()->writeWarning("No existe SpriteSheet para este tipo de entidad" + entity.name);
		return false;
	}
	auto candidate = it->second->targetRect(entity);
	return SDL_HasIntersection(&screenRect, &candidate);
}

void IsoView::addSpriteSheet(string name, string pPath, int pixelRefX, int pixelRefY, int altoSprite, int anchoSprite, int cantSprites, double fps, double delay) {
	auto it = spriteSheets.find(name);
	if (it != spriteSheets.end())
		Logger::getInstance()->writeError("Ya existe un spriteSheet para el tipo de entidad con nombre " + name);
	else {
		spriteSheets[name] = make_shared<SpriteSheet>(pPath, pixelRefX, pixelRefY, altoSprite, anchoSprite, cantSprites, fps, delay, *this);
		Logger::getInstance()->writeInformation("Se agrega spriteSheet para el tipo de entidad con nombre " + name);
	}
}

r2 IsoView::screenToBoardPosition(SDL_Point screenPos) {
	double XsTerm = (double)((double)screenPos.x - size.x / 2) / (double)TILE_WIDTH_DEFAULT;
	double YsTerm = (double)((double)screenPos.y - size.y / 2) / (double)TILE_HEIGHT_DEFAULT;
	return owner.getFocus() + r2(XsTerm + YsTerm + .5, -XsTerm + YsTerm + .5);
}

SDL_Point IsoView::boardToScreenPosition(r2 boardPos) {
	boardPos -= owner.getFocus();
	SDL_Point ret = {
		(int)(((boardPos.x - boardPos.y) * TILE_WIDTH_DEFAULT / 2) + (size.x) / 2),
		(int)(((boardPos.x + boardPos.y) * TILE_HEIGHT_DEFAULT / 2) + (size.y - TILE_HEIGHT_DEFAULT) / 2) };
	return ret;
}
