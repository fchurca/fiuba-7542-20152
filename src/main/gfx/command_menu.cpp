#include "game_window.h"
#include "command_menu.h"
#include "../parser_yaml/graphics_parser.h"
using namespace std;

CommandMenu::CommandMenu(GameWindow& owner, GraphicsParser& graphicsParser) : 
	owner(owner),
	offset(0, owner.alto_pantalla - graphicsParser.getPantalla().hud_alto),
	size((owner.ancho_pantalla-graphicsParser.getPantalla().minimapa_ancho)/2, graphicsParser.getPantalla().hud_alto)
{
}

CommandMenu::~CommandMenu() {
}

void CommandMenu::visit(Entity& entity) {
}
void CommandMenu::visit(Unit& entity) {
	visit((Entity&)entity);
	outText = outText + owner.completeLine("[] Ir", size.x);
	outText = outText + owner.completeLine("[] Atacar", size.x);
	outText = outText + owner.completeLine("[] Seguir", size.x);
	outText = outText + owner.completeLine("[] Parar", size.x);

}
void CommandMenu::visit(Worker& entity) {
	visit((Unit&)entity);
	outText = outText + owner.completeLine("[] Reparar", size.x);
	outText = outText + owner.completeLine("[] Recolectar", size.x);
	outText = outText + owner.completeLine("[] Construir", size.x);
	outText = outText + owner.completeLine("--> [] Posibles edificaciones", size.x);
}
void CommandMenu::visit(ProducerBuilding& entity) {
	visit((Entity&)entity);
	outText = outText + owner.completeLine("[] Producir", size.x);
	outText = outText + owner.completeLine("--> [] Posibles productos", size.x);
}

void CommandMenu::draw() {
	//Dibujo fondo
	SDL_Rect destinoFondoMenu = { (int)offset.x, (int)offset.y, (int)size.x, (int)size.y };
	SDL_SetRenderDrawColor(owner.getRenderer(), 15, 15, 15, 255);
	SDL_RenderFillRect(owner.getRenderer(), &destinoFondoMenu);
	outText = "";
	SDL_Color colorBlanco = { 255, 255, 255 };
	if (owner.font) {
		if (owner.getSelection().size() > 0) {
			owner.getSelection().at(0)->visit(*this);
		}
		int access1, w1, h1;
		Uint32 format1;
		SDL_Surface * c1 = TTF_RenderText_Blended_Wrapped(owner.font, outText.c_str(), colorBlanco, (Uint32)(size.x));
		SDL_Texture * textureMenu1 = SDL_CreateTextureFromSurface(owner.getRenderer(), c1);

		SDL_QueryTexture(textureMenu1, &format1, &access1, &w1, &h1);
		SDL_Rect panel1 = { 0, 0, w1 , h1 };
		SDL_Rect text1 = { (int)offset.x, (int)offset.y,
			(int)((w1 > size.x) ? size.x : w1), (int)((h1 > size.y) ? size.y : h1) };
		SDL_RenderCopy(owner.getRenderer(), textureMenu1, &panel1, &text1);

		SDL_FreeSurface(c1);
		SDL_DestroyTexture(textureMenu1);
	}
}