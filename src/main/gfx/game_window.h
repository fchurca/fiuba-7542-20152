#ifndef __GFX_GAMEWINDOW_H__

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "minimap.h"
#include "isoview.h"
#include "menu.h"
#include "chat.h"
#include "players_list.h"
#include "resources_list.h"
#include "command_menu.h"
#include "selection_menu.h"
#include "selection_controller.h"
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
	SDL_Point mouseDown;
	bool sweeping;
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
public:
	GameWindow(Game& owner, Player& player, GraphicsParser& graphicsParser, RulesetParser& rulesetParser);
	~GameWindow();
	int start();
	void update();
	void render();
	r2 getFocus();
	int alto_pantalla;
	int ancho_pantalla;
	void setSelection();
	ABoard& board;
	std::shared_ptr<MiniMap> minimap;
	std::shared_ptr<IsoView> isoview;
	std::shared_ptr<Menu> menu;
	std::shared_ptr<Chat> chat;
	std::shared_ptr<PlayersList> playersList;
	std::shared_ptr<ResourcesList> resourcesList;
	std::shared_ptr<CommandMenu> commandMenu;
	std::shared_ptr<SelectionMenu> selectionMenu;
	std::shared_ptr<SelectionController> sController;
	TTF_Font* font;
	std::string completeLine(std::string line, double width);
	SDL_Color getColor(int id);
	SDL_Renderer* getRenderer();
	bool isSweeping();
friend SpriteSheet;
};
#endif // __GFX_GAMEWINDOW_H__
