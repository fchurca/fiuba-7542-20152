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
	setFrame();
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
	while (!(command == 'L' || !socket->IsActive())) {
		*socket >> command;
		if(owner.willExit()) {
			(*socket << ack << frame << 'L').flushOut();
			break;
		}
		switch (command) {
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
				auto e = board.findEntity(i);
				if (e) {
					if (&(e->owner) == &(this->player)) {
						board.pushCommand(make_shared<MoveCommand>(e->getId(), r2(x, y)));
					}
				}
			}
			break;
		case 'U':
			{
				size_t frame;
				char eotSink;
				*socket >> frame >> eotSink;
				*socket << ack << this->frame;
				deletedMutex.lock();
				while (deleted.size() > 0) {
					*socket << 'D' << deleted.front();
					deleted.pop();
				}
				deletedMutex.unlock();
				board.mapEntities([this, frame](shared_ptr<Entity> e) {
						if(e) {
							if ((e->getFrame() > frame) && !e->getDeletable()) {
								*socket << 'E' << *e;
							}
						}}
						);
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
	setDeletable();
}

RemoteClient::~RemoteClient() {
	socket->deinit();
	if(running) {
		th.join();
	}
}

void RemoteClient::notifyDeath(int id) {
	auto board = this->owner.getBoard();
	deletedMutex.lock();
	deleted.push(id);
	deletedMutex.unlock();
}

