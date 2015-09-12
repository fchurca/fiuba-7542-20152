#include "game.h"

Game::Game(ParserYAML* parser){
	this->gameBoard = new Board(64, 48);
	this->gameBoard->buildBoard(parser);
}

Game::~Game(){
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

