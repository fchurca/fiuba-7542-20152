#include <algorithm>
#define NOMINMAX // Para que nadie nos redefina min max
#include <sstream>

#include "game_window.h"

using namespace std;

bool GameWindow::sdlInitialized = false;

bool GameWindow::initialize() {
	Logger::getInstance()->writeInformation("Initializing graphics");
	if (GameWindow::sdlInitialized) {
		Logger::getInstance()->writeWarning("SDL already initialized");
	} else {
		atexit(SDL_Quit);
		if( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {
			Logger::getInstance()->writeError("SDL could not initialize!");
			Logger::getInstance()->writeError(SDL_GetError());
			GameWindow::sdlInitialized = false;
		} else {
			GameWindow::sdlInitialized = true;
		}
	}
	return GameWindow::sdlInitialized;
}

GameWindow::GameWindow(Game& owner, Player& player, int sizeX, int sizeY, int scrollMargin, int scrollSpeed) :
	owner(owner), player(player), board(player.board),
	ancho_pantalla(sizeX), alto_pantalla(sizeY),
	margen_pantalla(scrollMargin), scroll_speed(scrollSpeed)
{
	GameWindow::initialize(); 
	window = SDL_CreateWindow("Trabajo Práctico 7542",
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		ancho_pantalla, alto_pantalla,
		SDL_WINDOW_SHOWN);

	Logger::getInstance()->writeInformation("Creating renderer");
	renderer = SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED );

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Color: negro opaco
	SDL_RenderClear(renderer); // Limpio pantalla inicialmente
	SDL_RenderPresent( renderer );
}

GameWindow::~GameWindow() {
	spriteSheets.clear();

	Logger::getInstance()->writeInformation("Destroying renderer");
	if (renderer) {
		SDL_DestroyRenderer(renderer);
		renderer = nullptr;
	}

	Logger::getInstance()->writeInformation("Destroying window");
	if (window) {
		SDL_DestroyWindow(window);
		window = nullptr;
	} else {
		Logger::getInstance()->writeWarning("Window never initialized");
	}
}

bool GameWindow::canDraw(Entity& entity) {
	if (!(&entity)) {
		return false;
	}
	SDL_Rect screenRect = {0, 0, ancho_pantalla, alto_pantalla};
	auto it = spriteSheets.find(entity.name);
	if (it == spriteSheets.end()) {
		Logger::getInstance()->writeWarning("No existe SpriteSheet para este tipo de entidad" + entity.name);
		return false;
	}
	auto candidate = it->second->targetRect(entity);
	return SDL_HasIntersection(&screenRect, &candidate);
}

void GameWindow::render() {
	//	Dibujar
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);
	// Dibujamos el terreno
	r2 margin(1,1),
	   ul = screenToBoardPosition({0, 0}) - margin, // Upper Left
	   ur = screenToBoardPosition({ancho_pantalla, 0}) - margin, // Upper Right
	   bl = screenToBoardPosition({0, alto_pantalla}) + margin, // Bottom Left
	   br = screenToBoardPosition({ancho_pantalla, alto_pantalla}) + margin; // Bottom Right
	double ud = ul.x + ul.y - 2, // Upper diagonal
		   bd = bl.x + bl.y + 2, // Bottom diagonal
		   ld = ul.x - ul.y - 2, // Left diagonal
		   rd = ur.x - ur.y + 2; // Right diagonal
	for (size_t x = max(0.0, ul.x),
			maxx = min(((double)board.sizeX) - 1, br.x);
			x < maxx;
			x++) {
		for (size_t y = max(max(max(0.0, ur.y), ud - x), x - rd),
				maxy = min(min(min(((double)board.sizeY) - 1, bl.y), bd - x), x - ld);
				y < maxy;
				y++) {
			if (y >= board.sizeY) {
				break;
			}
			Entity & tile = board.getTerrain(x, y);
			if (&tile) {
				if (canDraw(tile)) {
					spriteSheets[tile.name]->render(tile, renderer);
				}
			}
		}
		if (x >= board.sizeX) {
			break;
		}
	}
	// Seleccionamos entidades que se pisan con la pantalla
	auto entities = board.selectEntities([this](shared_ptr<Entity> e) {
			return canDraw(*e);});
	// Ordenamos las entidades por oclusión
	sort(entities.begin(), entities.end(), [](shared_ptr<Entity> a, shared_ptr<Entity> b) {
		return (a->size.x == a->size.y && b->size.x == b->size.y) ?
			(a->getX() + a->getY() + a->size.x < b->getX() + b->getY() + b->size.x) :
			((a->getX() + a->size.x < b->getX()) || (a->getY() + a->size.y < b->getY())) &&
			!((b->getX() + b->size.x <= a->getX()) || (b->getY() + b->size.y <= a->getY()));
	});
	for(auto& e : entities) {
		auto it = spriteSheets.find(e->name);
		if(it == spriteSheets.end()){
			Logger::getInstance()->writeWarning("No existe SpriteSheet para este tipo de entidad" + e->name);
			continue;
		}
		it->second->render(*e, renderer);
	}

	SDL_RenderPresent( renderer );
	return;
}

void GameWindow::init(){
	focus();
}

void GameWindow::update(){
	map<string, SpriteSheet*>::const_iterator itr;
	for(auto & kv : spriteSheets) {
		kv.second->update();
	}
	processInput();
	return;
}

void GameWindow::addSpriteSheet(string name, string pPath, int pixelRefX, int pixelRefY, int altoSprite, int anchoSprite, int cantSprites, double fps, double delay) {
	auto it = spriteSheets.find(name);
	if(it != spriteSheets.end())
		Logger::getInstance()->writeError("Ya existe un spriteSheet para el tipo de entidad con nombre " + name);
	else{
		spriteSheets[name] = make_shared<SpriteSheet>(pPath, pixelRefX, pixelRefY, altoSprite, anchoSprite, cantSprites, fps, delay, *this);
		Logger::getInstance()->writeInformation("Se agrega spriteSheet para el tipo de entidad con nombre " + name);
	}
}

r2 GameWindow::screenToBoardPosition(SDL_Point screenPos) {
	double XsTerm = (double)((double)screenPos.x - ancho_pantalla/2)/(double)TILE_WIDTH_DEFAULT;
	double YsTerm = (double)((double)screenPos.y - alto_pantalla/2)/(double)TILE_HEIGHT_DEFAULT;
	return focusPosition + r2(XsTerm + YsTerm + .5, -XsTerm + YsTerm + .5);
}

SDL_Point GameWindow::boardToScreenPosition(r2 boardPos) {
	boardPos -= focusPosition;
	SDL_Point ret = {
		(int)((boardPos.x - boardPos.y) * TILE_WIDTH_DEFAULT / 2) + (ancho_pantalla) / 2,
		(int)((boardPos.x + boardPos.y) * TILE_HEIGHT_DEFAULT / 2) + (alto_pantalla - TILE_HEIGHT_DEFAULT) / 2};
	return ret;
}

void GameWindow::processInput(){
	SDL_GetMouseState(&mouse.x, &mouse.y);
	scroll();
	//	Procesar input del usuario
	while(SDL_PollEvent(EventHandler::getInstance()->getEvent())) {
		auto & e = *(EventHandler::getInstance()->getEvent());
		switch(e.type) {
			case SDL_QUIT:
				owner.exit();
				break;
			case SDL_KEYDOWN:
				Logger::getInstance()->writeInformation("Teclado");
				if(e.key.keysym.sym == SDLK_r){
					owner.restart();
				}
				break;
			case SDL_MOUSEBUTTONUP:
				ostringstream oss;
				oss << "Mouse en " << mouse.x << "," << mouse.y;

				// Conversion de coordenadas en pantalla a coordenadas mapa

				auto mouseBoard = screenToBoardPosition(mouse);
				oss << "; mapa: " << mouseBoard.x << "," << mouseBoard.y;

				Logger::getInstance()->writeInformation(oss.str().c_str());
				if( EventHandler::getInstance()->getEvent()->button.button == SDL_BUTTON_LEFT ) {
					Logger::getInstance()->writeInformation("Boton Izquierdo");
					auto protagonist = getSelection();
					if (protagonist) {
						if (!(SDL_GetModState()&KMOD_SHIFT)) {
							protagonist->unsetTarget();
						}
						protagonist->addTarget(mouseBoard);
					}
				}
				if( EventHandler::getInstance()->getEvent()->button.button == SDL_BUTTON_RIGHT) {
					Logger::getInstance()->writeInformation("Boton derecho");
				}
				break;
		}
	}
}

void GameWindow::scroll(){
	double ds = (double)scroll_speed * (double)(board.dt) / 1000.0; //deltascroll
	r2 df;

	if(mouse.x <= margen_pantalla) {
		auto dsi = interpolate(mouse.x, 0, margen_pantalla, ds, 0);
		df += {-dsi, dsi};
		Logger::getInstance()->writeInformation("Scrolleando hacia la izquierda");
	}
	if(mouse.x >= ancho_pantalla - margen_pantalla){
		auto dsi = interpolate(mouse.x, ancho_pantalla - margen_pantalla, ancho_pantalla, 0, ds);
		df += {dsi, -dsi};
		Logger::getInstance()->writeInformation("Scrolleando hacia la derecha");
	}
	if(mouse.y <= margen_pantalla) {
		auto dsi = interpolate(mouse.y, 0, margen_pantalla, ds, 0);
		df += {-dsi, -dsi};
		Logger::getInstance()->writeInformation("Scrolleando hacia arriba");
	}
	if(mouse.y >= alto_pantalla - margen_pantalla) {
		auto dsi = interpolate(mouse.y, alto_pantalla - margen_pantalla, alto_pantalla, 0, ds);
		df += {dsi, dsi};
		Logger::getInstance()->writeInformation("Scrolleando hacia abajo");
	}
	focus(focusPosition + df);
}

void GameWindow::focus(r2 newFocus) {
	focusPosition.x = clip(newFocus.x, 0, board.sizeX - 1);
	focusPosition.y = clip(newFocus.y, 0, board.sizeY - 1);
}

void GameWindow::focus() {
	auto protagonist = getSelection();
	if (protagonist) {
		focus(protagonist->getPosition());
	}
}

r2 GameWindow::getFocus() {
	return focusPosition;
}

shared_ptr<Entity> GameWindow::getSelection() {
	return player.entities().front();
}

