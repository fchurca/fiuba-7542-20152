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
	istream& in = cin;
	*socket << ack << frame
		<< ht << player.getId() << ht << board.getPlayers().size()
		<< ht << board.name << lf;
	socket->flushOut();
/*
	for(auto& p : board.getPlayers()) {
		*socket << p->serialize();
	}
	*socket << "T\t" << board.sizeX << ht << board.sizeY << lf;
	// TODO: EntityFactories
	for(size_t x = board.sizeX - 1; x > 0; x--) {
		for(size_t y = board.sizeY - 1; y > 0; y--) {
			auto e = board.getTerrain(x, y);
			*socket << *e;
		}
	}
	*socket << "T" << lf;
	*socket << "Entities";
	board.mapEntities([this](shared_ptr<Entity> e) {*socket << *e;});
	socket->flushOut();*/
	string command;
	while (!(command == "L" || in.eof() || this->owner.willExit())) {
		bool ack = false;
		cerr << endl << ">";
		socket->flushIn();
		in >> command;
		if (command == "L") {
			ack = true;
		} else if (command == "S") {
			int i;
			in >> i;
			auto e = board.findEntity(i);
			if(e) {
				if (&(e->owner) == &(this->player)) {
					board.pushCommand(make_shared<StopCommand>(e->getId()));
					ack = true;
				}
			}
		} else if (command == "M") {
			int i;
			double x, y;
			in >> i >> x >> y;
			if (!this->owner.willExit()) {
				auto e = board.findEntity(i);
				if (e) {
					if (&(e->owner) == &(this->player)) {
						board.pushCommand(make_shared<MoveCommand>(e->getId(), r2(x, y)));
						ack = true;
					}
				}
			}
		} else if (command == "U") {
			size_t frame;
			in >> frame;
			*socket << this->frame;
			board.mapEntities([this, frame](shared_ptr<Entity> e) {
					if (e->getFrame() > frame) {
					*socket << *e;
					}
					});
			for(auto& p : board.getPlayers()) {
				if (p->getFrame() > frame) {
					*socket << p->serialize();
				}
			}
			deletedMutex.lock();
			while (deleted.size() > 0) {
				*socket << "D\t" << deleted.front() << lf;
				deleted.pop();
			}
			deletedMutex.unlock();
			ack = true;
		}
		if (ack) {
			// TODO: emprolijar
			socket->outBuffer.insert(socket->outBuffer.begin(), ack);
		} else {
			// TODO: emprolijar
			socket->outBuffer.clear();
			*socket << nak;
		}
		socket->flushOut();
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
	return socket << e.getId() << e.name
		<< e.getFrame()
		<< e.owner.getId()
		<< e.getPosition()
		<< e.getOrientation();
}

