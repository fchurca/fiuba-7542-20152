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
	SDL_Rect destinoFondoChat = { (int)offset.x, (int)offset.y, (int)size.x, (int)size.y };
	SDL_SetRenderDrawColor(renderer, 15, 15, 15, 255);
	SDL_RenderFillRect(renderer, &destinoFondoChat);

	//Me quedo con los ultimos mensajes
	while (messages.size() > maxMessages)
		messages.erase(messages.begin());

	//Mensajes anteriores
	std::string textMessages = "";
	SDL_Color colorBlanco = { 255, 255, 255 };
	if (owner.font) {
		for (int i = 0; i < maxMessages; i++) {
			if (messages.size() > i) {
				textMessages = textMessages + owner.completeLine(messages[i], size.x);
			}
		}
	}
	int access, w, h;
	Uint32 format;
	SDL_Surface * surface = TTF_RenderText_Blended_Wrapped(owner.font, textMessages.c_str(), colorBlanco, (Uint32)size.x);
	SDL_Texture * textureChat = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_QueryTexture(textureChat, &format, &access, &w, &h);
	SDL_Rect panel = { 0, 0, w , h };
	SDL_Rect text = { (int)offset.x, (int)offset.y,
		(int)((w>size.x) ? size.x : w), (int)((h>(maxMessages * size.y / (maxMessages + 1))) ? (maxMessages * size.y / (maxMessages + 1)) : h) };
	SDL_RenderCopy(renderer, textureChat, &panel, &text);
	SDL_FreeSurface(surface);
	SDL_DestroyTexture(textureChat);

	//Recuadro
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_Point points[] = {
		{ (int)offset.x, (int)(offset.y + maxMessages * size.y / (maxMessages + 1)) },
		{ (int)(offset.x+size.x),(int)(offset.y + maxMessages * size.y / (maxMessages + 1)) },
		{ (int)(offset.x + size.x) , (int)(offset.y+size.y) },
		{ (int)offset.x,(int)(offset.y + size.y) },
		{ (int)offset.x, (int)(offset.y + maxMessages * size.y / (maxMessages + 1)) } };
	SDL_RenderDrawLines(renderer, points, 5);

	//Nuevo mensaje
	std::string inputMessage;
	if (inputText != "")inputMessage = owner.completeLine(inputText, size.x);
	int inputAccess, inputW, inputH;
	Uint32 inputFormat;
	SDL_Surface * inputSurface = TTF_RenderText_Blended_Wrapped(owner.font, inputMessage.c_str(), colorBlanco, (Uint32)size.x);
	SDL_Texture * inputTexture = SDL_CreateTextureFromSurface(renderer, inputSurface);
	SDL_QueryTexture(inputTexture, &inputFormat, &inputAccess, &inputW, &inputH);
	SDL_Rect inputPanel = { 0, 0, inputW , inputH };
	SDL_Rect inputTextRect = { (int)(offset.x), (int)(offset.y + maxMessages * size.y / (maxMessages + 1)),
		(int)((inputW>size.x) ? size.x : inputW), (int)((inputH>size.y / (maxMessages + 1)) ? size.y / (maxMessages + 1) : inputH) };
	SDL_RenderCopy(renderer, inputTexture, &inputPanel, &inputTextRect);
	SDL_FreeSurface(inputSurface);
	SDL_DestroyTexture(inputTexture);
}
		