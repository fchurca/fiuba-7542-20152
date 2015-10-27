#ifndef __GFX_GAMEWINDOW_H__

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "minimap.h"
#include "isoview.h"
#include "../model/abstract_client.h"
#include "../model/game.h"
#include "../log/logger.h"
#include "../defines/defines.h"
#include "../controllers/event_handler.h"

class GraphicsParser;
class RulesetParser;

class GameWindow : public AClient {
private:
	std::string completeLine(std::string line, TTF_Font* font);
	void drawMenu();
protected:
	SDL_Point mouse;
	SDL_Window* window;
	SDL_Renderer* renderer;
	TTF_Font* font;
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
	ABoard& board;
	std::shared_ptr<Entity> selection;
public:
	GameWindow(Game& owner, Player& player, GraphicsParser& graphicsParser, RulesetParser& rulesetParser);
	~GameWindow();
	int start();
	void update();
	void render();
	r2 getFocus();
	int alto_pantalla;
	int ancho_pantalla;
	std::shared_ptr<Entity> getSelection();
	void clearSelection();
	void setSelection();
	bool selectionController();
	std::shared_ptr<MiniMap> minimap;
	std::shared_ptr<IsoView> isoview;
friend SpriteSheet;
};
#endif // __GFX_GAMEWINDOW_H__
