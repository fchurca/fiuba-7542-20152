#include "game_window.h"
#include <sstream>

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
	this->model = NULL;
	this->exit = false;
	this->focus_x = 0;
	this->focus_y = 0;

	Logger::getInstance()->writeInformation("Creating window");

	GameWindow::initialize(); 
	window = SDL_CreateWindow("Trabajo Práctico 7542",
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		ANCHO_DEFAULT, ALTO_DEFAULT,
		SDL_WINDOW_SHOWN);


	Logger::getInstance()->writeInformation("Creating renderer");
	renderer = SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED );


	// TODO: IMG_INIT

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
	if (renderer != NULL) {
		SDL_DestroyRenderer(renderer);
		renderer = NULL;
	}

	Logger::getInstance()->writeInformation("Destroying window");
	if (window != NULL) {
		Logger::getInstance()->writeWarning("Window never initialized");
		SDL_DestroyWindow(window);
		window = NULL;
	}
}

bool GameWindow::endOfGame(){
	return this->exit;
}

void GameWindow::render(){
	//	Dibujar
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);
	std::vector<std::shared_ptr<Entity>> entities = this->model->getBoard()->getEntities();
	std::map<std::string,SpriteSheet*>::iterator it;
	SpriteSheet* ss;
	for (std::size_t i =0; i < entities.size(); ++i){
		it = this->spritesSheets.find(entities[i]->name);
		if(it != this->spritesSheets.end()){
			ss = it->second;
			ss->render(*entities[i], 0, renderer);
		}
		else
			Logger::getInstance()->writeWarning("No existe SpriteSheet para este tipo de entidad" + entities[i]->name);
	}

	SDL_RenderPresent( renderer );
	return;
}

void GameWindow::restart(){
	//model->restart()	PARA NO TENER Q INSTANCIAR UN NUEVO MODEL
	delete model;

	map<std::string, SpriteSheet*>::const_iterator itr;
	for(itr = spritesSheets.begin(); itr != spritesSheets.end(); ++itr){
		delete itr->second;
	}

	init();
}

void GameWindow::init(){
	this->parser->parse();

	this->model = new Game(parser); // TODO: Esto debería ser parser->build()

	this->spritesSheets["agua"] = new SpriteSheet("resources//agua.png", 0, 0, TILE_HEIGHT_DEFAULT, TILE_WIDTH_DEFAULT,  1, 0, 0, *this);
	this->spritesSheets["pasto"] = new SpriteSheet("resources//pasto.png", 0, 0, TILE_HEIGHT_DEFAULT, TILE_WIDTH_DEFAULT, 1, 0, 0, *this);
	this->spritesSheets["piedra"] = new SpriteSheet("resources//piedra.png", 0, 0, TILE_HEIGHT_DEFAULT, TILE_WIDTH_DEFAULT, 1, 0, 0, *this);
	this->spritesSheets["chancho"] = new SpriteSheet("resources//chanchos.png", -14, 32, 44, 48, 15, 0, 1, *this);
}

void GameWindow::update(){
	model->update();
	return;
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

			double XsTerm = (double)(mouse_x_screen - ANCHO_DEFAULT/2)/(double)TILE_WIDTH_DEFAULT;
			double YsTerm = (double)(mouse_y_screen - ALTO_DEFAULT/2)/(double)TILE_HEIGHT_DEFAULT;
 
			double x_mapa = focus_x + XsTerm + YsTerm + .5;
			double y_mapa = focus_y - XsTerm + YsTerm + .5;

			oss << "; mapa: " << x_mapa << "," << y_mapa;

			Logger::getInstance()->writeInformation(oss.str().c_str());
			if( EventHandler::getInstance()->getEvent()->button.button == SDL_BUTTON_LEFT ) {
				Logger::getInstance()->writeInformation("Boton Izquierdo");
				model->getBoard()->getProtagonist().setTarget(x_mapa, y_mapa);
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
	const double SCROLL_SPEED = 5;

	double ds = SCROLL_SPEED * model->getBoard()->dt / 1000; //deltascroll
		SDL_GetMouseState(&mouse_x, &mouse_y);

		if(mouse_x <= MARGEN_PANTALLA_DEFAULT)
		{
			double dsi = (1.0 - ((double)mouse_x / (double)MARGEN_PANTALLA_DEFAULT)) * ds; 

			focus_x -= dsi;
			focus_y += dsi;
			Logger::getInstance()->writeInformation("Scrolleando hacia la izquierda");
		}
		else if(mouse_x >= ANCHO_DEFAULT - MARGEN_PANTALLA_DEFAULT){
			
			double dsi = ((double)(mouse_x + MARGEN_PANTALLA_DEFAULT - ANCHO_DEFAULT)/(double)MARGEN_PANTALLA_DEFAULT) * ds;

			focus_x += dsi;
			focus_y -= dsi;
			Logger::getInstance()->writeInformation("Scrolleando hacia la derecha");
		}
		if(mouse_y <= MARGEN_PANTALLA_DEFAULT)
		{
			double dsi = (1.0 - ((double)mouse_y / (double)MARGEN_PANTALLA_DEFAULT)) * ds;
			focus_x -= dsi;
			focus_y -= dsi;
			Logger::getInstance()->writeInformation("Scrolleando hacia arriba");
		}
		if(mouse_y >= ALTO_DEFAULT - MARGEN_PANTALLA_DEFAULT)
		{
			double dsi = ((double)(mouse_y + MARGEN_PANTALLA_DEFAULT - ALTO_DEFAULT)/(double)MARGEN_PANTALLA_DEFAULT) * ds;

			focus_x += dsi;
			focus_y += dsi;
			Logger::getInstance()->writeInformation("Scrolleando hacia abajo");
		}
		
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

int GameWindow::start(){
	init();
	{
		auto protagonist = model->getBoard()->getProtagonist();
		focus_x = protagonist.getX();
		focus_y = protagonist.getY();
	}

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


