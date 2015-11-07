#include "game_window.h"
#include "chat.h"

Chat::Chat(GameWindow& owner) :
	owner(owner),
	size(owner.ancho_pantalla / 3, owner.alto_pantalla / 3),
	offset(0, 0),
	maxMessages(5)
{
}

Chat::~Chat() {
}

void Chat::draw(SDL_Renderer* renderer) {
	//Dibujo fondo
	SDL_Rect destinoFondoChat = { offset.x, offset.y, size.x, size.y };
	SDL_SetRenderDrawColor(renderer, 15, 15, 15, 255);
	SDL_RenderFillRect(renderer, &destinoFondoChat);

	SDL_Color colorBlanco = { 255, 255, 255 };
	if (owner.font) {
	}
}
		