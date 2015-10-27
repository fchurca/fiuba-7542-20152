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
	stringstream out;
	istream& in = cin;
	out << ack << ht << frame
		<< ht << player.getId() << ht << board.getPlayers().size()
		<< ht << board.name << endl;

	for(auto& p : board.getPlayers()) {
		out << p->serialize();
	}
	out << "T\t" << board.sizeX << ht << board.sizeY<< endl;
	// TODO: EntityFactories
	for(size_t x = board.sizeX - 1; x > 0; x--) {
		for(size_t y = board.sizeY - 1; y > 0; y--) {
			auto e = board.getTerrain(x, y);
			out << serialize(*e);
		}
	}
	out << "T" << endl;
	out << "Entities";
	board.mapEntities([&out, this](shared_ptr<Entity> e) {out << serialize(*e);});
	string payload = out.str();
	socket->Send((void*)payload.c_str(), payload.length());
	cout << payload;
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
			answer << this->frame << ht;
			board.mapEntities([this, &answer, frame](shared_ptr<Entity> e) {
					if (e->getFrame() > frame) {
					answer << serialize(*e);
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
		socket->Send((void*)payload.c_str(), payload.length());
		cout << payload;
	}
}

RemoteClient::~RemoteClient() {
	if(running) {
		th.join();
	}
}

string RemoteClient::serialize(double d) {
	stringstream ret;
	ret << (int) (d * 100);
	return ret.str();
}

string RemoteClient::serialize(r2 r) {
	stringstream ret;
	ret << serialize(r.x) << ht << serialize(r.y);
	return ret.str();
}

string RemoteClient::serialize(Entity& e) {
	stringstream ret;
	ret << "E\t" << e.getId() << ht << e.name << ht
		<< e.getFrame() << ht
		<< e.owner.getId() << ht
		<< serialize(e.getPosition()) << ht
		<< serialize(e.getOrientation()) << endl;
	return ret.str();
}

