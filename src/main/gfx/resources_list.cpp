#include "game_window.h"
#include "resources_list.h"
#include "../parser_yaml/graphics_parser.h"

ResourcesList::ResourcesList(GameWindow& owner, GraphicsParser& graphicsParser) :
	owner(owner),
	offset(0, 0)
{
	int h, w;
	TTF_SizeText(owner.font, "A", &w, &h);
	size.x = owner.ancho_pantalla - graphicsParser.getPantalla().minimapa_ancho;
	size.y = h;
}

ResourcesList::~ResourcesList() {
}

void ResourcesList::draw() {
	//Dibujo fondo
	SDL_Rect destinoFondoMenu = { (int)offset.x, (int)offset.y, (int)size.x, (int)size.y };
	SDL_Color c = owner.getColor(owner.player.getId());
	SDL_SetRenderDrawColor(owner.getRenderer(), c.r * 0.65, c.g* 0.65, c.b* 0.65, 255);
	SDL_RenderFillRect(owner.getRenderer(), &destinoFondoMenu);
}