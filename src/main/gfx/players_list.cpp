#include "game_window.h"
#include "players_list.h"
#include "../parser_yaml/graphics_parser.h"

PlayersList::PlayersList(GameWindow& owner, GraphicsParser& graphicsParser) :
	owner(owner),
	size(graphicsParser.getPantalla().minimapa_ancho, owner.alto_pantalla -  graphicsParser.getPantalla().hud_alto),
	offset(owner.ancho_pantalla - graphicsParser.getPantalla().minimapa_ancho, 0)
{
}

PlayersList::~PlayersList() {
}

void PlayersList::draw() {
	//Dibujo fondo
	//SDL_Rect destinoFondoMenu = { (int)offset.x, (int)offset.y, (int)size.x, (int)size.y };
	//SDL_SetRenderDrawColor(owner.getRenderer(), 15, 15, 15, 255);
	//SDL_RenderFillRect(owner.getRenderer(), &destinoFondoMenu);
	if (owner.font) {
		std::string segundaColumnaActivos = "", segundaColumnaInactivos = "";
		SDL_Color colorBlanco = { 255, 255, 255 };
		SDL_Color colorGris = { 127, 127, 127 };
		//Segunda Columna//
		for (auto p : owner.player.board.getPlayers()) {
			if (!(p->getAlive())) {
				segundaColumnaInactivos = segundaColumnaInactivos + owner.completeLine("[X] " + p->name, size.x);
			}
			else {
				if (p->getActive()) {
					segundaColumnaActivos = segundaColumnaActivos + owner.completeLine("[A] " + p->name, size.x);
				}
				else {
					segundaColumnaInactivos = segundaColumnaInactivos + owner.completeLine("[I]  " + p->name, size.x);
				}
			}
		}

		int access2A, w2A, h2A, access2I, w2I, h2I;
		Uint32 format2A, format2I;
		SDL_Surface * c2A = TTF_RenderText_Blended_Wrapped(owner.font, segundaColumnaActivos.c_str(), colorBlanco, (Uint32)(size.x));
		SDL_Texture * textureMenu2A = SDL_CreateTextureFromSurface(owner.getRenderer(), c2A);
		SDL_Surface * c2I = TTF_RenderText_Blended_Wrapped(owner.font, segundaColumnaInactivos.c_str(), colorGris, (Uint32)(size.x));
		SDL_Texture * textureMenu2I = SDL_CreateTextureFromSurface(owner.getRenderer(), c2I);

		SDL_QueryTexture(textureMenu2A, &format2A, &access2A, &w2A, &h2A);
		SDL_Rect panel2A = { 0, 0, w2A, h2A };
		SDL_Rect text2A = { (int)(offset.x), (int)offset.y,
			(int)((w2A>size.x) ? size.x : w2A), (int)((h2A>size.y) ? size.y : h2A) };
		SDL_RenderCopy(owner.getRenderer(), textureMenu2A, &panel2A, &text2A);
		SDL_QueryTexture(textureMenu2I, &format2I, &access2I, &w2I, &h2I);
		if (segundaColumnaInactivos != "" && (size.y - h2A > h2I)) {
			SDL_Rect panel2I = { 0, 0, w2I, h2I };
			SDL_Rect text2I = { (int)(offset.x), (int)(h2A + offset.y),
				(int)((w2I>size.x) ? size.x : w2I), (int)h2I };
			SDL_RenderCopy(owner.getRenderer(), textureMenu2I, &panel2I, &text2I);
		}
		SDL_FreeSurface(c2A);
		SDL_FreeSurface(c2I);
		SDL_DestroyTexture(textureMenu2A);
		SDL_DestroyTexture(textureMenu2I);
	}
}