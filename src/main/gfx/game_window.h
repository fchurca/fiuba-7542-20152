#ifndef __GFX_GAMEWINDOW_H__

#include <SDL2/SDL.h>
#include "game.h"
#include "../log/logger.h"
#include "../defines/defines.h"
#include "../controllers/event_handler.h"
#include "sprite_sheet.h"

class GameWindow {
protected:
	SDL_Window* window; //= NULL;
	SDL_Renderer* renderer;// = NULL;
	std::map<std::string, std::shared_ptr<SpriteSheet>> spriteSheets;
	bool exit;
	unsigned int scroll_speed;
	unsigned int margen_pantalla;
	static bool sdlInitialized; // = false
	static bool initialize();
	void processInput();
	void scroll();
	r2 focusPosition;
	void focus(r2 position);
	void focus();
	bool canDraw(Entity& entity);
	Board& board;
	Game& owner;
public:
	void addSpriteSheet(std::string name, std::string pPath, int pixelRefX, int pixelRefY, int altoSprite, int anchoSprite, int cantSprites, double fps, double delay);
	GameWindow(Game& owner, Board& board, int sizeX, int sizeY, int scrollMargin, int scrollSpeed);
	~GameWindow();
	int start();
	void init();
	void update();
	void render();
	r2 getFocus();
	int alto_pantalla;
	int ancho_pantalla;
	r2 screenToBoardPosition(SDL_Point screenPos);
	SDL_Point boardToScreenPosition(r2 boardPos);

	friend Game;
};
#endif // __GFX_GAMEWINDOW_H__
