#include <iostream>
#include <sstream>

#include "remote_client.h"

#include "../model/board.h"
#include "../model/charnames.h"
#include "../model/entity.h"
#include "../model/game.h"

#include "../socket/socket.h"

using namespace std;
using namespace charnames;

#include <cstring>

void RemoteClient::setFrame() {
	auto board = owner.getBoard();
	if (board) {
		frame = board->getFrame();
	}
}

void RemoteClient::update() {
	auto board = this->owner.getBoard();
	deletedMutex.lock();
	board->mapEntities([this](shared_ptr<Entity> e) {
			if (e->getDeletable()) {
				deleted.push(e->getId());
			}
		});
	setFrame();
	deletedMutex.unlock();
	if (!running) {
		running = true;
		th = thread(&RemoteClient::run, this);
	}
}

RemoteClient::RemoteClient(Game& owner, Player& player, shared_ptr<Socket> socket) :
	AClient(owner, player), running(false), socket(socket)
{
	setFrame();
}

void RemoteClient::run() {
	auto& board = *owner.getBoard();
	*socket << ack << board.name << board.sizeX << board.sizeY << frame
		<< board.maxResources << player;
	for(auto p : board.getPlayers()) {
		if (p->getId() != player.getId()) {
			*socket << gs << *p;
		}
	}
	*socket << nul;
	for(size_t x = 0; x < board.sizeX; x++) {
		for(size_t y = 0; y < board.sizeY; y++) {
			auto e = board.getTerrain(x, y);
			*socket << e->name;
		}
	}
	board.mapEntities([this](shared_ptr<Entity> e) {*socket << gs << *e;});
	*socket << nul;
	socket->flushOut();
	char command = nul;
	while (!(command == 'L' || this->owner.willExit())) {
		cerr << "Listening to new command..." << endl;
		*socket >> command;
		switch (command) {
		case 'L':
			break;
		case 'S':
			{
				int i;
				char eotSink;
				*socket >> i >> eotSink;
				auto e = board.findEntity(i);
				if(e) {
					if (&(e->owner) == &(this->player)) {
						board.pushCommand(make_shared<StopCommand>(e->getId()));
					}
				}
			}
			break;
		case 'M':
			{
				int i;
				double x, y;
				char eotSink;
				*socket >> i >> x >> y >> eotSink;
				if (!this->owner.willExit()) {
					auto e = board.findEntity(i);
					if (e) {
						if (&(e->owner) == &(this->player)) {
							board.pushCommand(make_shared<MoveCommand>(e->getId(), r2(x, y)));
						}
					}
				}
			}
			break;
		case 'U':
			{
				size_t frame;
				char eotSink;
				*socket >> frame >> eotSink;
				cerr << "Update requested from frame " << frame
					<< ", request " << (eotSink == eot?"":"in") << "correctly terminated" << endl;
				*socket << ack << this->frame;
				board.mapEntities([this, frame](shared_ptr<Entity> e) {
						if (e->getFrame() > frame) {
						*socket << 'E' << *e;
						}
						});
				deletedMutex.lock();
				while (deleted.size() > 0) {
					*socket << 'D' << deleted.front();
					deleted.pop();
				}
				deletedMutex.unlock();
				for(auto& p : board.getPlayers()) {
					if (p->getFrame() > frame) {
						*socket << 'P' << *p;
					}
				}
				socket->flushOut();
			}
			break;
		}
	}
}

RemoteClient::~RemoteClient() {
	if(running) {
		th.join();
	}
}


Socket& operator<<(Socket& socket, r2 r) {
	return socket << r.x << r.y;
}

Socket& operator<<(Socket& socket, Entity& e) {
	return socket << e.getId() << e.name << e.owner.name
		<< e.getFrame()
		<< e.getPosition()
		<< e.getOrientation();
}

Socket& operator<<(Socket& socket, Player& p) {
	socket << p.name;
	for(auto& i : p.getResources()) {
		socket << gs << i.first << i.second;
		cerr << i.second << endl;
	}
	socket << nul;

	return socket;
}

