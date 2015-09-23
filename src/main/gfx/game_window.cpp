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
	this->parser = new ParserYAML(CONFIG_FILE_PATH);
	this->parser->parse();
	TagPantalla tp = this->parser->getPantalla();
	TagConfiguracion tc = this->parser->getConfiguracion();
	this->model = nullptr;
	this->exit = false;
	this->focusPosition.x = 0;
	this->focusPosition.y = 0;
	this->alto_pantalla = tp.alto;
	this->ancho_pantalla = tp.ancho;
	this->margen_pantalla = tc.margen_scroll;
	this->scroll_speed = tc.velocidad_scroll;

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
	map<string, SpriteSheet*>::const_iterator itr;
	for(itr = spriteSheets.begin(); itr != spriteSheets.end(); ++itr){
		delete itr->second;
	}

	delete parser;
	delete model;

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
	return this->exit;
}

bool GameWindow::canDraw(Entity& entity) {
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
	Board & board = *this->model->getBoard();
	// Dibujamos el terreno
	// TODO: Recorrer menos tablero
	auto ul = screenToBoardPosition({0, 0}); // Upper Left
	auto ur = screenToBoardPosition({ancho_pantalla, 0}); // Upper Right
	auto bl = screenToBoardPosition({0, alto_pantalla}); // Bottom Left
	auto br = screenToBoardPosition({ancho_pantalla, alto_pantalla}); // Bottom Right
	for (size_t x = max(0.0, ul.x); x < min((double)board.sizeX, br.x); x++) {
		for (size_t y = max(0.0, ur.y); y < min((double)board.sizeY, bl.y); y++) {
			Entity & tile = board.getTerrain(x, y);
			if (canDraw(tile)) {
				spriteSheets[tile.name]->render(tile, renderer);
			}
		}
	}
	// Seleccionamos entidades que se pisan con la pantalla
	auto entities = board.selectEntities([this](shared_ptr<Entity> e) {
			return this->canDraw(*e);});
	// Ordenamos las entidades por oclusión
	sort(entities.begin(), entities.end(), [](shared_ptr<Entity> a, shared_ptr<Entity> b) {
		return ((a->getX() + a->size.x <= b->getX()) || (a->getY() + a->size.y <= b->getY())) &&
			!((b->getX() + b->size.x <= a->getX()) || (b->getY() + b->size.y <= a->getY()));
	});
	for (size_t i =0; i < entities.size(); ++i){
		auto e = entities[i];
		auto it = spriteSheets.find(e->name);
		if(it == spriteSheets.end()){
			Logger::getInstance()->writeWarning("No existe SpriteSheet para este tipo de entidad" + e->name);
			continue;
		}
		auto ss = it->second;
		ss->render(*e, renderer);
	}

	SDL_RenderPresent( renderer );
	return;
}

void GameWindow::restart(){
	delete model;

	map<string, SpriteSheet*>::const_iterator itr;
	for(itr = spriteSheets.begin(); itr != spriteSheets.end(); ++itr){
		delete itr->second;
	}
	spriteSheets.clear();

	this->parser->parse();
	
	init();
}

void GameWindow::init(){ //NO DEBERIA INICIALIZARSE TODO ACA, ME DIO PROBLEMA DE REFERENCIAS LLEVARLO AL PARSER
	vector<TagTipoEntidad> tte = this->parser->getTiposEntidades();
	vector<TagTipoEntidad> ttt = this->parser->getTiposTerrenos();
	TagConfiguracion tc = this->parser->getConfiguracion();
	TagEscenario te = this->parser->getEscenario();
	this->model = new Game(te.size_x, te.size_y, tc.dt); 
	Board* board = this->model->getBoard();
	
	addSpriteSheet(ENTIDAD_DEFAULT_NOMBRE, ENTIDAD_DEFAULT_IMAGEN, ENTIDAD_DEFAULT_PIXEL_REF_X, ENTIDAD_DEFAULT_PIXEL_REF_Y, ENTIDAD_DEFAULT_ALTO_SPRITE, ENTIDAD_DEFAULT_ANCHO_SPRITE, ENTIDAD_DEFAULT_CANTIDAD_SPRITES, ENTIDAD_DEFAULT_FPS, ENTIDAD_DEFAULT_DELAY);
	board->createEntityFactory(ENTIDAD_DEFAULT_NOMBRE, r2(ENTIDAD_DEFAULT_ANCHO_BASE, ENTIDAD_DEFAULT_ALTO_BASE), 0);

	addSpriteSheet(TERRENO_DEFAULT_NOMBRE, TERRENO_DEFAULT_IMAGEN, TERRENO_DEFAULT_PIXEL_REF_X, TERRENO_DEFAULT_PIXEL_REF_Y, TERRENO_DEFAULT_ALTO_SPRITE, TERRENO_DEFAULT_ANCHO_SPRITE, TERRENO_DEFAULT_CANTIDAD_SPRITES, TERRENO_DEFAULT_FPS, TERRENO_DEFAULT_DELAY);
	board->createEntityFactory(TERRENO_DEFAULT_NOMBRE, r2(TERRENO_DEFAULT_ANCHO_BASE, TERRENO_DEFAULT_ALTO_BASE), 0);

	addSpriteSheet(PROTAGONISTA_DEFAULT_NOMBRE, PROTAGONISTA_DEFAULT_IMAGEN, PROTAGONISTA_DEFAULT_PIXEL_REF_X, PROTAGONISTA_DEFAULT_PIXEL_REF_Y, PROTAGONISTA_DEFAULT_ALTO_SPRITE, PROTAGONISTA_DEFAULT_ANCHO_SPRITE, PROTAGONISTA_DEFAULT_CANTIDAD_SPRITES, PROTAGONISTA_DEFAULT_FPS, PROTAGONISTA_DEFAULT_DELAY);
	board->createEntityFactory(PROTAGONISTA_DEFAULT_NOMBRE, r2(PROTAGONISTA_DEFAULT_ANCHO_BASE, PROTAGONISTA_DEFAULT_ALTO_BASE), VELOCIDAD_PERSONAJE_DEFAULT);

	for (size_t i =0; i < tte.size(); ++i){
		addSpriteSheet(tte[i].nombre, tte[i].imagen, tte[i].pixel_ref_x, tte[i].pixel_ref_y, tte[i].alto_sprite, tte[i].ancho_sprite,  tte[i].cantidad_sprites, tte[i].fps, tte[i].delay);
		board->createEntityFactory(tte[i].nombre, r2(tte[i].ancho_base, tte[i].alto_base), tc.vel_personaje); // LA VELOCIDAD DEBERIA IR SOLO AL PROTAGONISTA
	}

	for (size_t i =0; i < ttt.size(); ++i){
		addSpriteSheet(ttt[i].nombre, ttt[i].imagen, ttt[i].pixel_ref_x, ttt[i].pixel_ref_y, ttt[i].alto_sprite, ttt[i].ancho_sprite,  ttt[i].cantidad_sprites, ttt[i].fps, ttt[i].delay);
		board->createEntityFactory(ttt[i].nombre, r2(ttt[i].ancho_base, ttt[i].alto_base), 0); 
	}

	for(size_t i =0; i < te.terrenos.size(); ++i){
		board->setTerrain(te.terrenos[i].tipoEntidad,te.terrenos[i].pos_x,te.terrenos[i].pos_y);
	}
	if(!board->createProtagonist(te.protagonista.tipoEntidad, r2(te.protagonista.pos_x, te.protagonista.pos_y))){
		Logger::getInstance()->writeInformation("Se crea un protagonista default");
		board->createProtagonist(PROTAGONISTA_DEFAULT_NOMBRE, r2(PROTAGONISTA_DEFAULT_POSX, PROTAGONISTA_DEFAULT_POSY));
	}

	for(size_t i =0; i < te.entidades.size(); ++i){
		board->createEntity(te.entidades[i].tipoEntidad, r2(te.entidades[i].pos_x,te.entidades[i].pos_y));
	}

	for(size_t x = 0; x < board->sizeX; x++) {
		for(size_t y = 0; y < board->sizeY; y++) {
			if (!&board->getTerrain(x, y)) {
				board->setTerrain(TERRENO_DEFAULT_NOMBRE, x, y); // VER QUE EL PASTO NO DEBERIA VENIR EN EL ARCHIVO
			}
		}
	}

	focus();
}

void GameWindow::update(){
	GameTimer::update();
	map<string, SpriteSheet*>::const_iterator itr;
	for(itr = spriteSheets.begin(); itr != spriteSheets.end(); ++itr){
		itr->second->update();
	}
	model->update();
	return;
}

void GameWindow::addSpriteSheet(string name, string pPath, int pixelRefX, int pixelRefY, int altoSprite, int anchoSprite, int cantSprites, double fps, double delay) {
	map<string,SpriteSheet*>::iterator it;
	it = this->spriteSheets.find(name);
	if(it != this->spriteSheets.end())
		Logger::getInstance()->writeError("Ya existe un spriteSheet para el tipo de entidad con nombre " + name);
	else{
		spriteSheets[name] = new SpriteSheet(pPath, pixelRefX, pixelRefY, altoSprite, anchoSprite, cantSprites, fps, delay, *this);
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
	SDL_Point ret;
	ret.x = ((boardPos.x - boardPos.y) * TILE_WIDTH_DEFAULT / 2) + (ancho_pantalla) / 2;
	ret.y = ((boardPos.x + boardPos.y) * TILE_HEIGHT_DEFAULT / 2) + (alto_pantalla - TILE_HEIGHT_DEFAULT) / 2;
	return ret;
}

void GameWindow::processInput(){

	//	Procesar input del usuario
	while(SDL_PollEvent(EventHandler::getInstance()->getEvent())) {
		auto & e = *(EventHandler::getInstance()->getEvent());
		switch(e.type) {
			case SDL_QUIT:
				this->exit = true;
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
					auto protagonist = &(model->getBoard()->getProtagonist());
					if (protagonist) {
						protagonist->setTarget(mouseBoard);
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
	Uint8 mouse_b;
	int mouse_x, mouse_y;

	double ds = scroll_speed * model->getBoard()->dt / 1000; //deltascroll
	SDL_GetMouseState(&mouse_x, &mouse_y);
	double fx = focusPosition.x, fy = focusPosition.y;

	if(mouse_x <= margen_pantalla)
	{
		double dsi = (1.0 - ((double)mouse_x / (double)margen_pantalla)) * ds; 

		fx -= dsi;
		fy += dsi;
		Logger::getInstance()->writeInformation("Scrolleando hacia la izquierda");
	}
	else if(mouse_x >= ancho_pantalla - margen_pantalla){

		double dsi = ((double)(mouse_x + margen_pantalla - ancho_pantalla)/(double)margen_pantalla) * ds;

		fx += dsi;
		fy -= dsi;
		Logger::getInstance()->writeInformation("Scrolleando hacia la derecha");
	}
	if(mouse_y <= margen_pantalla)
	{
		double dsi = (1.0 - ((double)mouse_y / (double)margen_pantalla)) * ds;
		fx -= dsi;
		fy -= dsi;
		Logger::getInstance()->writeInformation("Scrolleando hacia arriba");
	}
	if(mouse_y >= alto_pantalla - margen_pantalla)
	{
		double dsi = ((double)(mouse_y + margen_pantalla - alto_pantalla)/(double)margen_pantalla) * ds;

		fx += dsi;
		fy += dsi;
		Logger::getInstance()->writeInformation("Scrolleando hacia abajo");
	}
	focus(r2(fx, fy));
}

void GameWindow::focus(r2 newFocus) {
	auto & board = *(model->getBoard());
	focusPosition.x = clip(newFocus.x, 0, board.sizeX - 1);
	focusPosition.y = clip(newFocus.y, 0, board.sizeY - 1);
}

void GameWindow::focus() {
	auto protagonist = &(model->getBoard()->getProtagonist());
	if (protagonist) {
		focus(protagonist->getPosition());
	}
}

r2 GameWindow::getFocus() {
	return focusPosition;
}

int GameWindow::start(){
	init();

	while (!endOfGame())
	{
		scroll();
		processInput();
		update();
		render();

		SDL_Delay(model->getBoard()->dt); // TODO: Optimizar, sacar hardcodeo
	}
	return 0;
}
