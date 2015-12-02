#include <sstream>

#include "game.h"

Game::Game() :
	exit_p(false)
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
	clients.clear();
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
	auto players = board->getPlayers();
	for(auto& p : players) {
		if(p->human && p->getAlive() && (clients.find(p->name) == clients.end())) {
			return p;
		}
	}
	return nullptr;
}

bool Game::addClient(std::shared_ptr<AClient> newClient) {
	auto& p = newClient->player;
	bool ret = true;
	clientsMutex.lock();
	if((&(newClient->owner) != this) ||
			(clients.find(p.name) != clients.end()) ||
			!p.human) {
		ret = false;
	}
	if(ret) {
		clients[p.name] = newClient;
		p.setActive(true);
	}
	clientsMutex.unlock();
	if(!getAvailablePlayer()) {
		board->setState(ABoard::BoardState::running);
	}
	return ret;
}

void Game::start() {
	board->start();
	while (!willExit()) {
		// Acá actualizamos a todos los clientes
		clientsMutex.lock();
		for(auto& c : clients) {
			c.second->update();
		}
		for(auto& i : clients) {
			if (i.second->getDeletable()) {
				i.second->player.setActive(false);
				clients.erase(i.second->player.name);
			}
		}
		clientsMutex.unlock();

		Logger::getInstance()->flush();
		if (!timer.elapse(board->dt)) {
			Logger::getInstance()->writeInformation("Estamos laggeando!");
		}
	}
}

void Game::exit() {
	exit_p = true;
	board->setState(ABoard::BoardState::finished);
}

bool Game::willExit() {
	return exit_p || (!board->isRunning());
}

void Game::notifyDeath(int id) {
	for (auto& c : clients) {
		c.second->notifyDeath(id);
	}
}

