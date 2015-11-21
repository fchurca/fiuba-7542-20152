#include "game_window.h"
#include "selection_menu.h"
#include "../parser_yaml/graphics_parser.h"
using namespace std;

SelectionMenu::SelectionMenu(GameWindow& owner, GraphicsParser& graphicsParser) :
	owner(owner),
	offset((owner.ancho_pantalla - graphicsParser.getPantalla().minimapa_ancho) / 2, owner.alto_pantalla - graphicsParser.getPantalla().hud_alto),
	size((owner.ancho_pantalla - graphicsParser.getPantalla().minimapa_ancho) / 2, graphicsParser.getPantalla().hud_alto)
{
}

SelectionMenu::~SelectionMenu() {
}

void SelectionMenu::visit(Entity& entity) {
	outText = outText + owner.completeLine(entity.name + "(" + entity.owner.name + ")", size.x);
}
void SelectionMenu::visit(Unit& entity) {
	visit((Entity&)entity);
	outText = outText + owner.completeLine("Salud: (--/--)", size.x);
}
void SelectionMenu::visit(Worker& entity) {
	visit((Unit&)entity);
	outText = outText + owner.completeLine("Recursos: (--/--)", size.x);
}
void SelectionMenu::visit(Resource& entity) {
	visit((Entity&)entity);
	outText = outText + owner.completeLine("Capacidad: (--/--)", size.x);
}
void SelectionMenu::visit(Building& entity) {
	visit((Entity&)entity);
	outText = outText + owner.completeLine("Salud: (--/--)", size.x);
}
void SelectionMenu::visit(UnfinishedBuilding& entity) {
	visit((Building&)entity);
	outText = outText + owner.completeLine("Progreso: (--/--)", size.x);
}
void SelectionMenu::visit(ProducerBuilding& entity) {
	visit((Building&)entity);
	outText = outText + owner.completeLine("Produccion: (--/--)", size.x);
}


void SelectionMenu::draw() {
	//Dibujo fondo
	SDL_Rect destinoFondoMenu = { (int)offset.x, (int)offset.y, (int)size.x, (int)size.y };
	SDL_SetRenderDrawColor(owner.getRenderer(), 15, 15, 15, 255);
	SDL_RenderFillRect(owner.getRenderer(), &destinoFondoMenu);
	outText = "";
	SDL_Color colorBlanco = { 255, 255, 255 };
	if (owner.font) {
		if (owner.getSelection().size() > 1) {
			for (auto e : owner.getSelection()) {
				outText = outText + owner.completeLine(e->name + "(" + e->owner.name + ")", size.x);
			}
		}
		else if (owner.getSelection().size() == 1) {
			owner.getSelection().at(0)->visit(*this);
		}
		int access3, w3, h3;
		Uint32 format3;
		SDL_Surface * c3 = TTF_RenderText_Blended_Wrapped(owner.font, outText.c_str(), colorBlanco, (Uint32)(size.x));
		SDL_Texture * textureMenu3 = SDL_CreateTextureFromSurface(owner.getRenderer(), c3);

		SDL_QueryTexture(textureMenu3, &format3, &access3, &w3, &h3);
		SDL_Rect panel3 = { 0, 0, w3, h3 };
		SDL_Rect text3 = { (int)(size.x), (int)(offset.y),
			(int)((w3>size.x) ? size.x : w3), (int)((h3>size.y) ? size.y : h3) };
		SDL_RenderCopy(owner.getRenderer(), textureMenu3, &panel3, &text3);
		SDL_FreeSurface(c3);
		SDL_DestroyTexture(textureMenu3);
	}
}