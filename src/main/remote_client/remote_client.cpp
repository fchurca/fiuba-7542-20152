#include "remote_client.h"
#include "../model/entity.h"
#include "../model/board.h"
#include "../model/game.h"

#include <iostream>

using namespace std;

void RemoteClient::setFrame() {
	auto board = owner.getBoard();
	if (board) {
		frame = board->getFrame();
	}
}

void RemoteClient::update() {
	auto board = owner.getBoard();
	if (board) {
		board->mapEntities([this](shared_ptr<Entity> e) {
				if (e->getFrame() > this->frame) {
				cerr << e->serialize();
				}
				});
		for(auto& p : board->getPlayers()) {
			if (p->getFrame() > frame) {
				cerr << p->serialize();
			}
		}
	}
	setFrame();
}

RemoteClient::RemoteClient(Game& owner, Player& player) :
	AClient(owner, player)
{
	setFrame();
	auto& board = *owner.getBoard();
	cerr << "Joining" << endl
		<< "Board " << board.name << endl
		<< player.serialize();
	for(auto& p : board.getPlayers()) {
		if (&player == &*p) {
			continue;
		}
		cerr << p->serialize();
	}
	cerr << "Terrain" << endl;
	for(size_t x = board.sizeX - 1; x > 0; x--) {
		for(size_t y = board.sizeY - 1; y > 0; y--) {
			auto e = board.getTerrain(x, y);
			cerr << e->serialize();
		}
	}
	cerr << "Entities";
	board.mapEntities([](shared_ptr<Entity> e) {
			cerr << e->serialize();});
}

