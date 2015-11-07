#ifndef __GFX_GAMEWINDOW_H__

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "minimap.h"
#include "isoview.h"
#include "menu.h"
#include "chat.h"
#include "../model/abstract_client.h"
#include "../model/game.h"
#include "../log/logger.h"
#include "../defines/defines.h"
#include "../controllers/event_handler.h"

class GraphicsParser;
class RulesetParser;

class GameWindow : public AClient {
protected:
	std::string inputText;
	SDL_Point mouse;
	SDL_Window* window;
	SDL_Renderer* renderer;
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
	std::shared_ptr<Menu> menu;
	std::shared_ptr<Chat> chat;
	TTF_Font* font;
	std::string completeLine(std::string line, double width);
friend SpriteSheet;
};
#endif // __GFX_GAMEWINDOW_H__
