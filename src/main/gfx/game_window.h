#ifndef __GFX_GAMEWINDOW_H__

#include <SDL2/SDL.h>
#include "../game.h"
#include "../log/logger.h"
#include "../defines/defines.h"
#include "../parser_yaml/parser_yaml.h"
#include "../controllers/event_handler.h"
#include "sprite_sheet.h"

class GameWindow {
protected:
	Game* model; //= NULL;
	ParserYAML* parser; //= NULL;
	SDL_Window* window; //= NULL;
	SDL_Renderer* renderer;// = NULL;
	std::map<std::string, SpriteSheet*> spritesSheets;
	bool exit;
	static bool sdlInitialized; // = false
	static bool initialize();
	bool endOfGame();
	void processInput();
	void update();
	void render();
	void restart();
	void scroll();
public:
	void addSpriteSheet(std::string name, std::string pPath, int pixelRefX, int pixelRefY, int altoSprite, int anchoSprite, int cantSprites, double fps, double delay);
	GameWindow();
	~GameWindow();
	int start();
	void init();
	double focus_x;
	double focus_y;
	unsigned int alto_pantalla;
	unsigned int ancho_pantalla;
};
#endif // __GFX_GAMEWINDOW_H__
