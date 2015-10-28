#include "game_window.h"
#include "menu.h"

Menu::Menu(GameWindow& owner) :
	owner(owner),
	size(3 * owner.ancho_pantalla / 4, owner.alto_pantalla / 4),
	offset(0, 3 * owner.alto_pantalla / 4)
{
	font = TTF_OpenFont(FUENTE_DEFAULT, 20);
	if (!font) {
		Logger::getInstance()->writeError("Error al abrir TTF");
	}
}

Menu::~Menu() {
	TTF_CloseFont(font);
}

std::string Menu::completeLine(std::string line, TTF_Font* font) {
	int txtAncho, txtAlto, espAncho, espAlto, esp;
	std::string result = line;
	TTF_SizeText(font, " ", &espAncho, &espAlto);
	TTF_SizeText(font, result.c_str(), &txtAncho, &txtAlto);
	esp = floor((size.x / 3 - txtAncho) / espAncho);
	if (esp * espAncho + txtAncho < size.x / 3)
		esp++;
	result.insert(result.size(), esp, ' ');
	return result;
}

void Menu::draw(SDL_Renderer* renderer) {
	if (font) {
		std::string primerColumna = "", segundaColumnaActivos = "", segundaColumnaInactivos = "", terceraColumna = "";
		SDL_Color colorBlanco = { 255, 255, 255 };
		SDL_Color colorGris = { 127, 127, 127 };
		//Primer Columna//
		primerColumna = completeLine(owner.player.name, font);
		for (auto r : owner.player.getResources()) {
			primerColumna = primerColumna + completeLine(r.first + "=" + std::to_string(r.second), font);
		}
		//
		//Segunda Columna//
		for (auto p : owner.player.board.getPlayers()) {
			if (owner.player.getActive())
				segundaColumnaActivos = segundaColumnaActivos + completeLine(p->name, font);
			else
				segundaColumnaInactivos = segundaColumnaInactivos + completeLine(p->name, font);
		}

		//Tercer Columna//
		std::shared_ptr<Entity> s = owner.getSelection();
		if (s != nullptr) {
			terceraColumna = terceraColumna + completeLine(s->name, font);
			terceraColumna = terceraColumna + completeLine("(" + s->owner.name + ")", font);
		}
		int access1, w1, h1, access2A, w2A, h2A, access2I, w2I, h2I, access3, w3, h3;
		Uint32 format1, format2A, format2I, format3;
		SDL_Surface * c1 = TTF_RenderText_Blended_Wrapped(font, primerColumna.c_str(), colorBlanco, size.x / 3);
		SDL_Texture * textureMenu1 = SDL_CreateTextureFromSurface(renderer, c1);
		SDL_Surface * c2A = TTF_RenderText_Blended_Wrapped(font, segundaColumnaActivos.c_str(), colorBlanco, size.x / 3);
		SDL_Texture * textureMenu2A = SDL_CreateTextureFromSurface(renderer, c2A);
		SDL_Surface * c2I = TTF_RenderText_Blended_Wrapped(font, segundaColumnaInactivos.c_str(), colorGris, size.x / 3);
		SDL_Texture * textureMenu2I = SDL_CreateTextureFromSurface(renderer, c2I);
		SDL_Surface * c3 = TTF_RenderText_Blended_Wrapped(font, terceraColumna.c_str(), colorBlanco, size.x / 3);
		SDL_Texture * textureMenu3 = SDL_CreateTextureFromSurface(renderer, c3);

		SDL_QueryTexture(textureMenu1, &format1, &access1, &w1, &h1);
		// Si w1 o h1 extienden el ancho_pantalla/4 o alto_pantalla/4 respectivamente poner esos como max
		SDL_Rect panel1 = { 0, 0, w1 , h1 };
		SDL_Rect text1 = { offset.x, offset.y, w1 , h1 };
		SDL_RenderCopy(renderer, textureMenu1, &panel1, &text1);

		SDL_QueryTexture(textureMenu2A, &format2A, &access2A, &w2A, &h2A);
		// Si w2 o h2 extienden el ancho_pantalla/4 o alto_pantalla/4 respectivamente poner esos como max
		SDL_Rect panel2A = { 0, 0, w2A, h2A };
		SDL_Rect text2A = { size.x / 3, offset.y, w2A, h2A };
		SDL_RenderCopy(renderer, textureMenu2A, &panel2A, &text2A);
		if (segundaColumnaInactivos != "") {
			SDL_QueryTexture(textureMenu2I, &format2I, &access2I, &w2I, &h2I);
			SDL_Rect panel2I = { 0, 0, w2I, h2I };
			SDL_Rect text2I = { size.x/3, h2A + offset.y, w2I, h2I };
			SDL_RenderCopy(renderer, textureMenu2I, &panel2I, &text2I);
		}

		SDL_QueryTexture(textureMenu3, &format3, &access3, &w3, &h3);
		// Si w3 o h3 extienden el ancho_pantalla/4 o alto_pantalla/4 respectivamente poner esos como max
		SDL_Rect panel3 = { 0, 0, w3, h3 };
		SDL_Rect text3 = { 2 * size.x/3, offset.y, w3, h3 };
		SDL_RenderCopy(renderer, textureMenu3, &panel3, &text3);
		SDL_FreeSurface(c1);
		SDL_FreeSurface(c2A);
		SDL_FreeSurface(c2I);
		SDL_FreeSurface(c3);
		SDL_DestroyTexture(textureMenu1);
		SDL_DestroyTexture(textureMenu2A);
		SDL_DestroyTexture(textureMenu2I);
		SDL_DestroyTexture(textureMenu3);
	}
}