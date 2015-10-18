#ifndef __GFX_GAMEWINDOW_H__

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "sprite_sheet.h"
#include "../model/abstract_client.h"
#include "../model/game.h"
#include "../log/logger.h"
#include "../defines/defines.h"
#include "../controllers/event_handler.h"

class ParserYAML;

class GameWindow : public AClient {
private:
	std::string completeLine(std::string line, TTF_Font* font);
protected:
	SDL_Point mouse;
	SDL_Window* window; //= NULL;
	SDL_Renderer* renderer;// = NULL;
	std::map<std::string, std::shared_ptr<SpriteSheet>> spriteSheets;
	unsigned int scroll_speed;
	unsigned int margen_pantalla;
	static bool sdlInitialized; // = false
	static bool initialize();
	void processInput();
	void scroll();
	r2 focusPosition;
	r2 boardMouse;
	void focus(r2 position);
	void focus();
	bool canDraw(std::shared_ptr<Entity> entity);
	Board& board;
	std::shared_ptr<Entity> selection;
public:
	void addSpriteSheet(std::string name, std::string pPath, int pixelRefX, int pixelRefY, int altoSprite, int anchoSprite, int cantSprites, double fps, double delay);
	GameWindow(Game& owner, Player& player, ParserYAML& parser);
	~GameWindow();
	int start();
	void update();
	void render();
	r2 getFocus();
	int alto_pantalla;
	int ancho_pantalla;
	r2 screenToBoardPosition(SDL_Point screenPos);
	SDL_Point boardToScreenPosition(r2 boardPos);
	std::shared_ptr<Entity> getSelection();
	void clearSelection();
	void setSelection();
	bool selectionController();
friend SpriteSheet;
};
#endif // __GFX_GAMEWINDOW_H__
