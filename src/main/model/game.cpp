#include <sstream>

#include "game.h"
#include "../parser_yaml/parser_yaml.h"
#include "../gfx/game_timer.h"

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
	client = nullptr;
	restart_p = false;
	exit_p = false;
}

bool Game::setBoard(std::shared_ptr<ABoard> newBoard) {
	if (board) {
		return false;
	}
	board = newBoard;
	return true;
}

std::shared_ptr<ABoard> Game::getBoard() {
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

bool Game::addClient(std::shared_ptr<AClient> newClient) {
	client = newClient;
	return true;
}

void Game::start() {
	while (!willExit()) {
		GameTimer::update();
		board->update(); // Model
		client->update(); // Controller

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

