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

GameWindow::GameWindow() {
	parser = make_shared<ParserYAML>(CONFIG_FILE_PATH);
	parser->parse();
	TagPantalla tp = parser->getPantalla();
	TagConfiguracion tc = parser->getConfiguracion();
	model = nullptr;
	exit = false;
	focusPosition.x = 0;
	focusPosition.y = 0;
	alto_pantalla = tp.alto;
	ancho_pantalla = tp.ancho;
	margen_pantalla = tc.margen_scroll;
	scroll_speed = tc.velocidad_scroll;

	Logger::getInstance()->writeInformation("Creating window");

	GameWindow::initialize(); 
	window = SDL_CreateWindow("Trabajo Práctico 7542",
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		tp.ancho, tp.alto,
		SDL_WINDOW_SHOWN);

	Logger::getInstance()->writeInformation("Creating renderer");
	renderer = SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED );

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Color: negro opaco
	SDL_RenderClear(renderer); // Limpio pantalla inicialmente
	SDL_RenderPresent( renderer );
}

GameWindow::~GameWindow() {
	spriteSheets.clear();

	parser = nullptr;
	Logger::getInstance()->writeInformation("Destroying renderer");
	if (renderer) {
		SDL_DestroyRenderer(renderer);
		renderer = nullptr;
	}

	Logger::getInstance()->writeInformation("Destroying window");
	if (window) {
		SDL_DestroyWindow(window);
	} else {
		Logger::getInstance()->writeWarning("Window never initialized");
	}
}

bool GameWindow::endOfGame(){
	return exit;
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
	auto& board = getBoard();
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

void GameWindow::restart(){
	model = nullptr;

	spriteSheets.clear();

	parser->parse();
	
	init();
}

void GameWindow::init(){ //NO DEBERIA INICIALIZARSE TODO ACA, ME DIO PROBLEMA DE REFERENCIAS LLEVARLO AL PARSER
	auto tc = parser->getConfiguracion();
	auto te = parser->getEscenario();

	model = make_shared<Game>(te.size_x, te.size_y, tc.dt); 
	auto& board = getBoard();
	
	addSpriteSheet(ENTIDAD_DEFAULT_NOMBRE, ENTIDAD_DEFAULT_IMAGEN, ENTIDAD_DEFAULT_PIXEL_REF_X, ENTIDAD_DEFAULT_PIXEL_REF_Y, ENTIDAD_DEFAULT_ALTO_SPRITE, ENTIDAD_DEFAULT_ANCHO_SPRITE, ENTIDAD_DEFAULT_CANTIDAD_SPRITES, ENTIDAD_DEFAULT_FPS, ENTIDAD_DEFAULT_DELAY);
	board.createEntityFactory(ENTIDAD_DEFAULT_NOMBRE, {ENTIDAD_DEFAULT_ANCHO_BASE, ENTIDAD_DEFAULT_ALTO_BASE}, 0);

	addSpriteSheet(TERRENO_DEFAULT_NOMBRE, TERRENO_DEFAULT_IMAGEN, TERRENO_DEFAULT_PIXEL_REF_X, TERRENO_DEFAULT_PIXEL_REF_Y, TERRENO_DEFAULT_ALTO_SPRITE, TERRENO_DEFAULT_ANCHO_SPRITE, TERRENO_DEFAULT_CANTIDAD_SPRITES, TERRENO_DEFAULT_FPS, TERRENO_DEFAULT_DELAY);
	board.createEntityFactory(TERRENO_DEFAULT_NOMBRE, {TERRENO_DEFAULT_ANCHO_BASE, TERRENO_DEFAULT_ALTO_BASE}, 0);

	addSpriteSheet(PROTAGONISTA_DEFAULT_NOMBRE, PROTAGONISTA_DEFAULT_IMAGEN, PROTAGONISTA_DEFAULT_PIXEL_REF_X, PROTAGONISTA_DEFAULT_PIXEL_REF_Y, PROTAGONISTA_DEFAULT_ALTO_SPRITE, PROTAGONISTA_DEFAULT_ANCHO_SPRITE, PROTAGONISTA_DEFAULT_CANTIDAD_SPRITES, PROTAGONISTA_DEFAULT_FPS, PROTAGONISTA_DEFAULT_DELAY);
	board.createEntityFactory(PROTAGONISTA_DEFAULT_NOMBRE, {PROTAGONISTA_DEFAULT_ANCHO_BASE, PROTAGONISTA_DEFAULT_ALTO_BASE}, VELOCIDAD_PERSONAJE_DEFAULT);

	for(auto& t : parser->getTiposEntidades()) {
		addSpriteSheet(t.nombre, t.imagen, t.pixel_ref_x, t.pixel_ref_y, t.alto_sprite, t.ancho_sprite,  t.cantidad_sprites, t.fps, t.delay);
		board.createEntityFactory(t.nombre, {t.ancho_base, t.alto_base}, tc.vel_personaje); // LA VELOCIDAD DEBERIA IR SOLO AL PROTAGONISTA
	}

	for(auto& t : parser->getTiposTerrenos()) {
		addSpriteSheet(t.nombre, t.imagen, t.pixel_ref_x, t.pixel_ref_y, t.alto_sprite, t.ancho_sprite,  t.cantidad_sprites, t.fps, t.delay);
		board.createEntityFactory(t.nombre, {t.ancho_base, t.alto_base}, 0); 
	}

	for(auto& t : te.terrenos) {
		board.setTerrain(t.tipoEntidad, t.pos_x, t.pos_y);
	}

	if(!board.createProtagonist(te.protagonista.tipoEntidad, {(double)te.protagonista.pos_x, (double)te.protagonista.pos_y})){
		Logger::getInstance()->writeInformation("Se crea un protagonista default");
		board.createProtagonist(PROTAGONISTA_DEFAULT_NOMBRE, {PROTAGONISTA_DEFAULT_POSX, PROTAGONISTA_DEFAULT_POSY});
	}

	for(auto& t : te.entidades) {
		board.createEntity(t.tipoEntidad, {(double)t.pos_x,(double)t.pos_y});
	}

	for(size_t x = 0; x < board.sizeX; x++) {
		for(size_t y = 0; y < board.sizeY; y++) {
			if (!&board.getTerrain(x, y)) {
				board.setTerrain(TERRENO_DEFAULT_NOMBRE, x, y); // VER QUE EL PASTO NO DEBERIA VENIR EN EL ARCHIVO
			}
		}
	}

	focus();
}

void GameWindow::update(){
	GameTimer::update();
	map<string, SpriteSheet*>::const_iterator itr;
	for(auto & kv : spriteSheets) {
		kv.second->update();
	}
	model->update();
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

	//	Procesar input del usuario
	while(SDL_PollEvent(EventHandler::getInstance()->getEvent())) {
		auto & e = *(EventHandler::getInstance()->getEvent());
		switch(e.type) {
			case SDL_QUIT:
				exit = true;
				break;
			case SDL_KEYDOWN:
				Logger::getInstance()->writeInformation("Teclado");
				if(e.key.keysym.sym == SDLK_r){
					restart();
				}
				break;
			case SDL_MOUSEBUTTONUP:
				SDL_Point mouse_screen;
				SDL_GetMouseState(&mouse_screen.x, &mouse_screen.y);
				ostringstream oss;
				oss << "Mouse en " << mouse_screen.x << "," << mouse_screen.y;

				// Conversion de coordenadas en pantalla a coordenadas mapa

				auto mouseBoard = screenToBoardPosition(mouse_screen);
				oss << "; mapa: " << mouseBoard.x << "," << mouseBoard.y;

				Logger::getInstance()->writeInformation(oss.str().c_str());
				if( EventHandler::getInstance()->getEvent()->button.button == SDL_BUTTON_LEFT ) {
					Logger::getInstance()->writeInformation("Boton Izquierdo");
					auto protagonist = &(getBoard().getProtagonist());
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
	double ds = (double)scroll_speed * (double)(getBoard().dt) / 1000.0; //deltascroll
	r2 df;

	int mouse_x, mouse_y;
	SDL_GetMouseState(&mouse_x, &mouse_y);
	if(mouse_x <= margen_pantalla) {
		auto dsi = interpolate(mouse_x, 0, margen_pantalla, ds, 0);
		df += {-dsi, dsi};
		Logger::getInstance()->writeInformation("Scrolleando hacia la izquierda");
	}
	if(mouse_x >= ancho_pantalla - margen_pantalla){
		auto dsi = interpolate(mouse_x, ancho_pantalla - margen_pantalla, ancho_pantalla, 0, ds);
		df += {dsi, -dsi};
		Logger::getInstance()->writeInformation("Scrolleando hacia la derecha");
	}
	if(mouse_y <= margen_pantalla) {
		auto dsi = interpolate(mouse_y, 0, margen_pantalla, ds, 0);
		df += {-dsi, -dsi};
		Logger::getInstance()->writeInformation("Scrolleando hacia arriba");
	}
	if(mouse_y >= alto_pantalla - margen_pantalla) {
		auto dsi = interpolate(mouse_y, alto_pantalla - margen_pantalla, alto_pantalla, 0, ds);
		df += {dsi, dsi};
		Logger::getInstance()->writeInformation("Scrolleando hacia abajo");
	}
	focus(focusPosition + df);
}

void GameWindow::focus(r2 newFocus) {
	auto & board = getBoard();
	focusPosition.x = clip(newFocus.x, 0, board.sizeX - 1);
	focusPosition.y = clip(newFocus.y, 0, board.sizeY - 1);
}

void GameWindow::focus() {
	auto protagonist = &(getBoard().getProtagonist());
	if (protagonist) {
		focus(protagonist->getPosition());
	}
}

r2 GameWindow::getFocus() {
	return focusPosition;
}

Board& GameWindow::getBoard() {
	return *(model->getBoard());
}

int GameWindow::start(){
	init();

	while (!endOfGame()) {
		scroll();
		processInput();
		update();
		render();

		int dt = getBoard().dt;
		if (!GameTimer::wait(GameTimer::getCurrent() + dt)) {
			Logger::getInstance()->writeInformation("Estamos laggeando!");
		}
	}
	return 0;
}
