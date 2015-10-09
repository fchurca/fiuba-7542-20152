#include <sstream>

#include "game.h"
#include "../gfx/game_window.h"
#include "../parser_yaml/parser_yaml.h"

Game::Game() :
	exit_p(false), restart_p(false)
{
	std::stringstream message;
	message << "Creating Game " << this;
	Logger::getInstance()->writeInformation(message.str());
}

Game::~Game(){
	std::stringstream message;
	message << "Killing Game " << this;
	Logger::getInstance()->writeInformation(message.str());
	clear();
}

void Game::clear() {
	board = nullptr;
	gameWindow = nullptr;
	restart_p = false;
	exit_p = false;
}

bool Game::setBoard(std::shared_ptr<Board> newBoard) {
	board = newBoard;
}

std::shared_ptr<Board> Game::getBoard() {
	return board;
}

std::shared_ptr<Player> Game::getAvailablePlayer() {
	// TODO: Trazar jugadores disponibles
	auto players = board->getPlayers();
	for(auto& p : players) {
		if(p->human) {
			return p;
		}
	}
	return nullptr;
}

bool Game::addClient(std::shared_ptr<GameWindow> newClient) {
	gameWindow = newClient;
}

void Game::start() {
	while (!willExit()) {
		GameTimer::update();
		board->update(); // Model
		gameWindow->update(); // Controller

		Logger::getInstance()->flush();
		if (!GameTimer::wait(GameTimer::getCurrent() + board->dt)) {
			Logger::getInstance()->writeInformation("Estamos laggeando!");
		}
	}
}

void Game::restart() {
	restart_p = true;
}

bool Game::willRestart() {
	return restart_p;
}

void Game::exit() {
	exit_p = true;
}

bool Game::willExit() {
	return exit_p or restart_p;
}

