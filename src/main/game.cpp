#include <sstream>

#include "game.h"

Game::Game(int sizeX, int sizeY){
	std::stringstream message;
	message << "Creating Game " << this;
	Logger::getInstance()->writeInformation(message.str());
	gameBoard = new Board(sizeX, sizeY, 50); // TODO: Traer de configuración
	gameBoard->buildBoard();
}

Game::~Game(){
	std::stringstream message;
	message << "Killing Game " << this;
	Logger::getInstance()->writeInformation(message.str());
	delete(this->gameBoard);
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

Board* Game::getBoard(){
	return gameBoard;
}

