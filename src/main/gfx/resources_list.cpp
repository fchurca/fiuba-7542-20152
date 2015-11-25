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
	SDL_Color color = owner.getColor(owner.player.getId());
	SDL_SetRenderDrawColor(owner.getRenderer(), color.r * 0.65, color.g* 0.65, color.b* 0.65, 255);
	SDL_RenderFillRect(owner.getRenderer(), &destinoFondoMenu);
	SDL_Color colorBlanco = { 255, 255, 255 };
	std::string texto = owner.player.name + " | ";
	for (auto r : owner.player.getResources()) {
		texto = texto + "| "+r.first + ": " + std::to_string(r.second) + " ";
	}
	texto = owner.completeLine(texto, size.x);
	int access, w, h;
	Uint32 format;
	SDL_Surface * c = TTF_RenderText_Blended_Wrapped(owner.font, texto.c_str(), colorBlanco, (Uint32)(size.x));
	SDL_Texture * textureMenu = SDL_CreateTextureFromSurface(owner.getRenderer(), c);
	SDL_QueryTexture(textureMenu, &format, &access, &w, &h);
	SDL_Rect panel = { 0, 0, w, h };
	SDL_Rect text = { (int)(offset.x), (int)offset.y,
		(int)((w>size.x) ? size.x : w), (int)((h>size.y) ? size.y : h) };
	SDL_RenderCopy(owner.getRenderer(), textureMenu, &panel, &text);
	SDL_FreeSurface(c);
	SDL_DestroyTexture(textureMenu);
}