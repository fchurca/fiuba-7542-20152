#include "game.h"

Game::Game(int sizeX, int sizeY){
	this->gameBoard = new Board(sizeX, sizeY);
	this->gameBoard->buildBoard();
}

Game::~Game(){
	std::cerr << "Killing Game " << this << std::endl;
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
	return this->gameBoard;
}

