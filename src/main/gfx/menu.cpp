#include "game_window.h"
#include "menu.h"
#include "../parser_yaml/graphics_parser.h"

Menu::Menu(GameWindow& owner, GraphicsParser& graphicsParser) :
	owner(owner),
	size(owner.ancho_pantalla - graphicsParser.getPantalla().minimapa_ancho, graphicsParser.getPantalla().hud_alto),
	offset(0, owner.alto_pantalla - graphicsParser.getPantalla().hud_alto)
{
}

Menu::~Menu() {
}


void Menu::draw() {
	//Dibujo fondo
	SDL_Rect destinoFondoMenu = { (int)offset.x, (int)offset.y, (int)size.x, (int)size.y };
	SDL_SetRenderDrawColor(owner.getRenderer(), 15, 15, 15, 255);
	SDL_RenderFillRect(owner.getRenderer(), &destinoFondoMenu);
	if (owner.font) {
		std::string primerColumna = "", terceraColumna = "";
		SDL_Color colorBlanco = { 255, 255, 255 };
		//Primer Columna//
		primerColumna = owner.completeLine(owner.player.name, size.x / 2);
		for (auto r : owner.player.getResources()) {
			primerColumna = primerColumna + owner.completeLine(r.first + "=" + std::to_string(r.second), size.x / 2);
		}

		//Tercer Columna//
		for (auto e : owner.getSelection()){
			terceraColumna = terceraColumna + owner.completeLine(e->name + "(" + e->owner.name + ")", size.x / 2);
		}
		int access1, w1, h1, access3, w3, h3;
		Uint32 format1, format2A, format2I, format3;
		SDL_Surface * c1 = TTF_RenderText_Blended_Wrapped(owner.font, primerColumna.c_str(), colorBlanco, (Uint32)(size.x / 2));
		SDL_Texture * textureMenu1 = SDL_CreateTextureFromSurface(owner.getRenderer(), c1);
		SDL_Surface * c3 = TTF_RenderText_Blended_Wrapped(owner.font, terceraColumna.c_str(), colorBlanco, (Uint32)(size.x / 2));
		SDL_Texture * textureMenu3 = SDL_CreateTextureFromSurface(owner.getRenderer(), c3);

		SDL_QueryTexture(textureMenu1, &format1, &access1, &w1, &h1);
		SDL_Rect panel1 = { 0, 0, w1 , h1 };
		SDL_Rect text1 = { (int)offset.x, (int)offset.y,
			(int)((w1>size.x/2)? size.x/2 : w1), (int)((h1>size.y) ? size.y : h1) };
		SDL_RenderCopy(owner.getRenderer(), textureMenu1, &panel1, &text1);

		SDL_QueryTexture(textureMenu3, &format3, &access3, &w3, &h3);
		SDL_Rect panel3 = { 0, 0, w3, h3 };
		SDL_Rect text3 = { (int)(size.x/2), (int)(offset.y),
			(int)((w3>size.x / 2) ? size.x / 2 : w3), (int)((h3>size.y) ? size.y : h3) };
		SDL_RenderCopy(owner.getRenderer(), textureMenu3, &panel3, &text3);
		SDL_FreeSurface(c1);
		SDL_FreeSurface(c3);
		SDL_DestroyTexture(textureMenu1);
		SDL_DestroyTexture(textureMenu3);
	}
}
