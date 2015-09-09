#include "game.h"

Game::Game(Board* map){
	this->gameBoard = map;
	this->exitGame = false;
}

Game::~Game(){
	delete(EventHandler::getInstance());
	delete(this->gameBoard);
}

bool Game::endOfGame(){
	return this->exitGame;
}

void Game::render(){
	//	Dibujar
	return;
}

void Game::update(){
	//	Actualizar juego
	return;
}

void Game::processInput(){
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

void Game::init(){
	//	Cargar recusos, configuracion, pantalla, etc

	return;
}

int Game::start(){
	GameWindow window = GameWindow();
	init();
	while (!endOfGame())
	{
		processInput();
		update();
		render();

		//sleep();
	}
	return 0;
}
