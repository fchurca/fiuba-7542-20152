#include "remote_client.h"
#include "../model/entity.h"
#include "../model/board.h"
#include "../model/game.h"
#include "../socket/socket.h"

#include <iostream>
#include <sstream>

using namespace std;

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
	ostream& out = cout;
	istream& in = cin;
	out << "+\t" << frame
		<< '\t' << player.getId() << '\t' << board.getPlayers().size()
		<< '\t' << board.name << endl;

	for(auto& p : board.getPlayers()) {
		out << p->serialize();
	}
	out << "T\t" << board.sizeX << '\t' << board.sizeY<< endl;
	// TODO: EntityFactories
	for(size_t x = board.sizeX - 1; x > 0; x--) {
		for(size_t y = board.sizeY - 1; y > 0; y--) {
			auto e = board.getTerrain(x, y);
			out << e->serialize();
		}
	}
	out << "T" << endl;
	out << "Entities";
	board.mapEntities([&out](shared_ptr<Entity> e) {out << e->serialize();});
	string command;
	while (!(command == "L" || in.eof() || this->owner.willExit())) {
		bool ack = false;
		cerr << endl << ">";
		stringstream answer;
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
			answer << this->frame << '\t';
			board.mapEntities([this, &answer, frame](shared_ptr<Entity> e) {
					if (e->getFrame() > frame) {
					answer << e->serialize();
					}
					});
			for(auto& p : board.getPlayers()) {
				if (p->getFrame() > frame) {
					answer << p->serialize();
				}
			}
			deletedMutex.lock();
			while (deleted.size() > 0) {
				answer << "D\t" << deleted.front() << endl;
				deleted.pop();
			}
			deletedMutex.unlock();
			ack = true;
		}
		string payload;
		if (ack) {
			payload = "+" + answer.str() + "\n";
		} else {
			payload = "-\n";
		}
		out << payload;
	}
}

RemoteClient::~RemoteClient() {
	if(running) {
		th.join();
	}
}
