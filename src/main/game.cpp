#include "game.h"

Game::Game(Board* map){
	this->gameBoard = map;
}

Game::~Game(){
	// delete(this->gameBoard); NO MIENTRAS GAMEBOARD SEA RESPONSABILIDAD DE GAME
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

