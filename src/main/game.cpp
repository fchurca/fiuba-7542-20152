#include "game.h"

Game::Game(){
	this->gameBoard = new Board(100, 100);
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

