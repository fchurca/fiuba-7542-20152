#include <sstream>

#include "game.h"

Game::Game(int sizeX, int sizeY, size_t dt){
	std::stringstream message;
	message << "Creating Game " << this;
	Logger::getInstance()->writeInformation(message.str());
	gameBoard = make_shared<Board>(sizeX, sizeY, dt); 
	gameBoard->buildBoard();
}

Game::~Game(){
	std::stringstream message;
	message << "Killing Game " << this;
	Logger::getInstance()->writeInformation(message.str());
	gameBoard = nullptr;
}

void Game::init(){
	//	Cargar recusos, configuracion, pantalla, etc

	return;
}

void Game::update(){
	//	Actualizar juego
	gameBoard->update();
	return;
}

shared_ptr<Board> Game::getBoard(){
	return gameBoard;
}

