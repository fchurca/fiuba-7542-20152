#include "game_window.h"
#include <sstream>
#include <algorithm>

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
	this->focus_x = 0;
	this->focus_y = 0;
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
	map<std::string, SpriteSheet*>::const_iterator itr;
	for(itr = spritesSheets.begin(); itr != spritesSheets.end(); ++itr){
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

void GameWindow::render(){
	//	Dibujar
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);
	Board & board = *this->model->getBoard();
	// Dibujamos el terreno
	for (size_t x = 0; x < board.sizeX; x++) {
		for (size_t y = 0; y < board.sizeY; y++) {
			Entity & tile = board.getTerrain(x, y);
			spritesSheets[tile.name]->render(tile, 0, renderer, this->alto_pantalla, this->ancho_pantalla);
		}
	}
	std::vector<std::shared_ptr<Entity>> entities = board.getEntities();
	std::map<std::string,SpriteSheet*>::iterator it;
	SpriteSheet* ss;
	// Ordenamos las entidades por oclusión
	std::sort(entities.begin(), entities.end(), [](std::shared_ptr<Entity> a, std::shared_ptr<Entity> b) {
		return ((a->getX() + a->size.x <= b->getX()) || (a->getY() + a->size.y <= b->getY())) &&
			!((b->getX() + b->size.x <= a->getX()) || (b->getY() + b->size.y <= a->getY()));
	});
	for (std::size_t i =0; i < entities.size(); ++i){
		it = this->spritesSheets.find(entities[i]->name);
		if(it != this->spritesSheets.end()){
			ss = it->second;
			ss->render(*entities[i], 0, renderer, this->alto_pantalla, this->ancho_pantalla);
		}
		else
			Logger::getInstance()->writeWarning("No existe SpriteSheet para este tipo de entidad" + entities[i]->name);
	}

	SDL_RenderPresent( renderer );
	return;
}

void GameWindow::restart(){
	delete model;

	map<std::string, SpriteSheet*>::const_iterator itr;
	for(itr = spritesSheets.begin(); itr != spritesSheets.end(); ++itr){
		delete itr->second;
	}
	spritesSheets.clear();

	this->parser->parse();
	
	init();
}

void GameWindow::init(){ //NO DEBERIA INICIALIZARSE TODO ACA, ME DIO PROBLEMA DE REFERENCIAS LLEVARLO AL PARSER
	std::vector<TagTipoEntidad> tte = this->parser->getTiposEntidades();
	std::vector<TagTipoEntidad> ttt = this->parser->getTiposTerrenos();
	TagConfiguracion tc = this->parser->getConfiguracion();
	TagEscenario te = this->parser->getEscenario();
	this->model = new Game(te.size_x, te.size_y, tc.dt); 
	Board* board = this->model->getBoard();
	
	addSpriteSheet(ENTIDAD_DEFAULT_NOMBRE, ENTIDAD_DEFAULT_IMAGEN, ENTIDAD_DEFAULT_PIXEL_REF_X, ENTIDAD_DEFAULT_PIXEL_REF_Y, ENTIDAD_DEFAULT_ALTO_SPRITE, ENTIDAD_DEFAULT_ANCHO_SPRITE, ENTIDAD_DEFAULT_CANTIDAD_SPRITES, ENTIDAD_DEFAULT_FPS, ENTIDAD_DEFAULT_DELAY);
	board->createEntityFactory(ENTIDAD_DEFAULT_NOMBRE, ENTIDAD_DEFAULT_ANCHO_BASE, ENTIDAD_DEFAULT_ALTO_BASE, 0);

	addSpriteSheet(TERRENO_DEFAULT_NOMBRE, TERRENO_DEFAULT_IMAGEN, TERRENO_DEFAULT_PIXEL_REF_X, TERRENO_DEFAULT_PIXEL_REF_Y, TERRENO_DEFAULT_ALTO_SPRITE, TERRENO_DEFAULT_ANCHO_SPRITE, TERRENO_DEFAULT_CANTIDAD_SPRITES, TERRENO_DEFAULT_FPS, TERRENO_DEFAULT_DELAY);
	board->createEntityFactory(TERRENO_DEFAULT_NOMBRE, TERRENO_DEFAULT_ANCHO_BASE, TERRENO_DEFAULT_ALTO_BASE, 0);

	addSpriteSheet(PROTAGONISTA_DEFAULT_NOMBRE, PROTAGONISTA_DEFAULT_IMAGEN, PROTAGONISTA_DEFAULT_PIXEL_REF_X, PROTAGONISTA_DEFAULT_PIXEL_REF_Y, PROTAGONISTA_DEFAULT_ALTO_SPRITE, PROTAGONISTA_DEFAULT_ANCHO_SPRITE, PROTAGONISTA_DEFAULT_CANTIDAD_SPRITES, PROTAGONISTA_DEFAULT_FPS, PROTAGONISTA_DEFAULT_DELAY);
	board->createEntityFactory(PROTAGONISTA_DEFAULT_NOMBRE, PROTAGONISTA_DEFAULT_ANCHO_BASE, PROTAGONISTA_DEFAULT_ALTO_BASE, VELOCIDAD_PERSONAJE_DEFAULT);

	for (std::size_t i =0; i < tte.size(); ++i){
		addSpriteSheet(tte[i].nombre, tte[i].imagen, tte[i].pixel_ref_x, tte[i].pixel_ref_y, tte[i].alto_sprite, tte[i].ancho_sprite,  tte[i].cantidad_sprites, tte[i].fps, tte[i].delay);
		board->createEntityFactory(tte[i].nombre, tte[i].ancho_base, tte[i].alto_base,tc.vel_personaje); // LA VELOCIDAD DEBERIA IR SOLO AL PROTAGONISTA
	}

	for (std::size_t i =0; i < ttt.size(); ++i){
		addSpriteSheet(ttt[i].nombre, ttt[i].imagen, ttt[i].pixel_ref_x, ttt[i].pixel_ref_y, ttt[i].alto_sprite, ttt[i].ancho_sprite,  ttt[i].cantidad_sprites, ttt[i].fps, ttt[i].delay);
		board->createEntityFactory(ttt[i].nombre, ttt[i].ancho_base, ttt[i].alto_base, 0); 
	}

	for(std::size_t i =0; i < te.terrenos.size(); ++i){
		board->setTerrain(te.terrenos[i].tipoEntidad,te.terrenos[i].pos_x,te.terrenos[i].pos_y); // ACA TENDRIA QE VALIDARSE SUPERPOSICION
	}
	if(!board->createProtagonist(te.protagonista.tipoEntidad,te.protagonista.pos_x, te.protagonista.pos_y)){
		Logger::getInstance()->writeInformation("Se crea un protagonista default");
		board->createProtagonist(PROTAGONISTA_DEFAULT_NOMBRE,PROTAGONISTA_DEFAULT_POSX, PROTAGONISTA_DEFAULT_POSY);
	}

	for(std::size_t i =0; i < te.entidades.size(); ++i){
		board->createEntity(te.entidades[i].tipoEntidad,te.entidades[i].pos_x,te.entidades[i].pos_y); // ACA TENDRIA QE VALIDARSE SUPERPOSICION
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
	map<std::string, SpriteSheet*>::const_iterator itr;
	for(itr = spritesSheets.begin(); itr != spritesSheets.end(); ++itr){
		itr->second->update();
	}
	model->update();
	return;
}

void GameWindow::addSpriteSheet(std::string name, std::string pPath, int pixelRefX, int pixelRefY, int altoSprite, int anchoSprite, int cantSprites, double fps, double delay) {
	std::map<std::string,SpriteSheet*>::iterator it;
	it = this->spritesSheets.find(name);
	if(it != this->spritesSheets.end())
		Logger::getInstance()->writeError("Ya existe un spriteSheet para el tipo de entidad con nombre " + name);
	else{
		spritesSheets[name] = new SpriteSheet(pPath, pixelRefX, pixelRefY, altoSprite, anchoSprite, cantSprites, fps, delay, *this);
		Logger::getInstance()->writeInformation("Se agrega spriteSheet para el tipo de entidad con nombre " + name);
	}
}

void GameWindow::processInput(){
	int mouse_x_screen, mouse_y_screen;

	//	Procesar input del usuario
	while(SDL_PollEvent(EventHandler::getInstance()->getEvent())) {
		if(EventHandler::getInstance()->getEvent()->type == SDL_QUIT )
			this->exit = true;
		if(EventHandler::getInstance()->getEvent()->type == SDL_KEYDOWN ){
			Logger::getInstance()->writeInformation("Teclado");
			if(EventHandler::getInstance()->getEvent()->key.keysym.sym == SDLK_r){
				restart();
			}
		}
		if( EventHandler::getInstance()->getEvent()->type == SDL_MOUSEBUTTONUP ){
			SDL_GetMouseState(&mouse_x_screen, &mouse_y_screen);
			std::ostringstream oss;
			oss << "Mouse en " << mouse_x_screen << "," << mouse_y_screen;

			// Conversion de coordenadas en pantalla a coordenadas mapa

			double XsTerm = (double)((double)mouse_x_screen - ancho_pantalla/2)/(double)TILE_WIDTH_DEFAULT;
			double YsTerm = (double)((double)mouse_y_screen - alto_pantalla/2)/(double)TILE_HEIGHT_DEFAULT;

			double x_mapa = focus_x + XsTerm + YsTerm + .5;
			double y_mapa = focus_y - XsTerm + YsTerm + .5;

			oss << "; mapa: " << x_mapa << "," << y_mapa;

			Logger::getInstance()->writeInformation(oss.str().c_str());
			if( EventHandler::getInstance()->getEvent()->button.button == SDL_BUTTON_LEFT ) {
				Logger::getInstance()->writeInformation("Boton Izquierdo");
				auto protagonist = &(model->getBoard()->getProtagonist());
				if (protagonist) {
					protagonist->setTarget(x_mapa, y_mapa);
				}
			}
			if( EventHandler::getInstance()->getEvent()->button.button == SDL_BUTTON_RIGHT) {
				Logger::getInstance()->writeInformation("Boton derecho");
			}
		}
	}
}

void GameWindow::scroll(){
	Uint8 mouse_b;
	int mouse_x, mouse_y;

	double ds = scroll_speed * model->getBoard()->dt / 1000; //deltascroll
	SDL_GetMouseState(&mouse_x, &mouse_y);
	double fx = focus_x, fy = focus_y;

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
	focus(fx, fy);
}

void GameWindow::focus(int x, int y) {
	focus_x = x;
	focus_y = y;
	auto & board = *(model->getBoard());
	if(focus_x >= board.sizeX - 1){
		focus_x = board.sizeX - 1;
	}
	else if(focus_x < 0){
		focus_x = 0;
	}

	if(focus_y >= board.sizeY - 1){
		focus_y = board.sizeY - 1;
	}else if(focus_y < 0){
		focus_y = 0;
	}
}

void GameWindow::focus() {
	auto protagonist = &(model->getBoard()->getProtagonist());
	if (protagonist) {
		focus(protagonist->getX(), protagonist->getY());
	}
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
