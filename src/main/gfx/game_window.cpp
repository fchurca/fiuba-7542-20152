#include "game_window.h"

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
	this->exitGame = false;
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
	return this->exitGame;
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
			if(entities[i]->name == "chancho")
				ss->dibujarAnimado(entities[i]->getX() * TILE_WIDTH_DEFAULT, entities[i]->getY() * TILE_HEIGHT_DEFAULT, renderer);
			else
				ss->render(entities[i]->getX() * TILE_WIDTH_DEFAULT, entities[i]->getY() * TILE_HEIGHT_DEFAULT, renderer);
		}
		else
			Logger::getInstance()->writeWarning("No existe SpriteSheet para este tipo de entidad" + entities[i]->name);
	}
	
	SDL_RenderPresent( renderer );
	return;
}

void GameWindow::init(){
	this->parser->parse();
	
	this->model = new Game(parser);

	this->spritesSheets["agua"] = new SpriteSheet("resources//agua.png", 0, 0, TILE_WIDTH_DEFAULT, TILE_HEIGHT_DEFAULT, 1, 0, 0);
	this->spritesSheets["pasto"] = new SpriteSheet("resources//pasto.png", 0, 0, TILE_WIDTH_DEFAULT, TILE_HEIGHT_DEFAULT, 1, 0, 0);
	this->spritesSheets["piedra"] = new SpriteSheet("resources//piedra.png", 0, 0, TILE_WIDTH_DEFAULT, TILE_HEIGHT_DEFAULT, 1, 0, 0);
	this->spritesSheets["chancho"] = new SpriteSheet("resources//chanchos.png", 0, 0, 44, 48, 15, 0, 0);

}

void GameWindow::update(){
	model->update();
	return;
}

void GameWindow::processInput(){
	//	Procesar input del usuario
	if(SDL_PollEvent(EventHandler::getInstance()->getEvent())) {
		if(EventHandler::getInstance()->getEvent()->type == SDL_QUIT )
			this->exitGame = true;
		if(EventHandler::getInstance()->getEvent()->type == SDL_KEYDOWN )
			Logger::getInstance()->writeInformation("Teclado");
		if( EventHandler::getInstance()->getEvent()->type == SDL_MOUSEBUTTONUP ){
			if( EventHandler::getInstance()->getEvent()->button.button == SDL_BUTTON_LEFT )
				Logger::getInstance()->writeInformation("Boton Izquierdo");
			if( EventHandler::getInstance()->getEvent()->button.button == SDL_BUTTON_RIGHT)
				Logger::getInstance()->writeInformation("Boton derecho");
		}
	}
}

int GameWindow::start(){
	init();

	
	while (!endOfGame())
	{
		processInput();
		update();
		render();

		SDL_Delay(100); // TODO: Optimizar, sacar hardcodeo
	}
	return 0;
}
	

