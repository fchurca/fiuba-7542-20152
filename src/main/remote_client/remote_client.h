#ifndef _REMOTE_CLIENT_H_
#define _REMOTE_CLIENT_H_

#include "../model/abstract_client.h"
#include "../parser_yaml/parser_yaml.h"

class RemoteClient : public AClient{
	protected:
		size_t frame;
		void setFrame();
	public:
		void update();
		RemoteClient(Game& owner, Player& player);
};

// THIS GOES TO remote_client.cpp
#include "../model/entity.h"
#include "../model/board.h"
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

#endif // _REMOTE_CLIENT_H_
