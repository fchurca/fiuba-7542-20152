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
	messages.clear();
}

void Chat::draw(SDL_Renderer* renderer, std::string inputText) {
	//Dibujo fondo
	SDL_Rect destinoFondoChat = { offset.x, offset.y, size.x, size.y };
	SDL_SetRenderDrawColor(renderer, 15, 15, 15, 255);
	SDL_RenderFillRect(renderer, &destinoFondoChat);

	//Me quedo con los ultimos mensajes
	while (messages.size() > maxMessages)
		messages.erase(messages.begin());

	//Texto
	std::string textMessages = "";
	SDL_Color colorBlanco = { 255, 255, 255 };
	if (owner.font) {
		for (int i = 0; i < maxMessages; i++) {
			if (messages.size() > i) {
				textMessages = textMessages + owner.completeLine(messages[i], size.x);
			}
		}
	}
	textMessages = textMessages + owner.completeLine(inputText, size.x);
	int access, w, h;
	Uint32 format;
	SDL_Surface * surface = TTF_RenderText_Blended_Wrapped(owner.font, textMessages.c_str(), colorBlanco, size.x);
	SDL_Texture * textureChat = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_QueryTexture(textureChat, &format, &access, &w, &h);
	SDL_Rect panel = { 0, 0, w , h };
	SDL_Rect text = { (int)offset.x, (int)offset.y,
		(int)((w>size.x / 3) ? size.x : w), (int)((h>size.y) ? size.y : h) };
	SDL_RenderCopy(renderer, textureChat, &panel, &text);
	SDL_FreeSurface(surface);
	SDL_DestroyTexture(textureChat);
}
		