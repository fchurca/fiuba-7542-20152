#include <sstream>

#include "game.h"
#include "game_window.h"

Game::Game(GameWindow* gameWindow) : gameWindow(gameWindow) {
	std::stringstream message;
	message << "Creating Game " << this;
	Logger::getInstance()->writeInformation(message.str());
	auto& parser = gameWindow->getParser();
	auto te = parser.getEscenario();
	auto tc = parser.getConfiguracion();
	gameBoard = make_shared<Board>(te.size_x, te.size_y, tc.dt); 
	gameBoard->buildBoard();
}

Game::~Game(){
	std::stringstream message;
	message << "Killing Game " << this;
	Logger::getInstance()->writeInformation(message.str());
	gameBoard = nullptr;
	gameWindow = nullptr;
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

