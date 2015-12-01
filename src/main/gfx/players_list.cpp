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
		std::string columna = "";
		std::string gameMode = "Modo de Juego: ";
		SDL_Color colorBlanco = { 255, 255, 255 };
		if (owner.player.board.gameMode == KILL_KING) {
			gameMode = gameMode + "Matar al rey";
		}
		else {
			if (owner.player.board.gameMode == DESTROY_CENTER) {
				gameMode = gameMode + "Destruir centro civico";
			}
			else {
				if (owner.player.board.gameMode == DESTROY_FLAG) {
					gameMode = gameMode + "Destruir bandera";
				}
			}
		}
		columna = columna + owner.completeLine(gameMode, size.x);
		for (auto p : owner.player.board.getPlayers()) {
			if (p->name != DEFAULT_PLAYER_NAME) {
				if (!(p->getAlive())) {
					columna = columna + owner.completeLine("[X] " + p->name, size.x);
				}
				else {
					if (p->getActive()) {
						columna = columna + owner.completeLine("[A] " + p->name, size.x);
					}
					else {
						columna = columna + owner.completeLine("[I]  " + p->name, size.x);
					}
				}
			}
		}

		int access, w, h;
		Uint32 format;
		SDL_Surface * c = TTF_RenderText_Blended_Wrapped(owner.font, columna.c_str(), colorBlanco, (Uint32)(size.x));
		SDL_Texture * textureMenu = SDL_CreateTextureFromSurface(owner.getRenderer(), c);

		SDL_QueryTexture(textureMenu, &format, &access, &w, &h);
		SDL_Rect panel = { 0, 0, w, h };
		SDL_Rect text = { (int)(offset.x), (int)offset.y,
			(int)((w>size.x) ? size.x : w), (int)((h>size.y) ? size.y : h) };
		SDL_RenderCopy(owner.getRenderer(), textureMenu, &panel, &text);
		SDL_FreeSurface(c);
		SDL_DestroyTexture(textureMenu);
	}
}