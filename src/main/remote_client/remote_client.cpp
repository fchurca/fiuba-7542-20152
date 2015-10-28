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
	*this << ack << ht << frame
		<< ht << player.getId() << ht << board.getPlayers().size()
		<< ht << board.name << lf;
	send();

	for(auto& p : board.getPlayers()) {
		*this << p->serialize();
	}
	*this << "T\t" << board.sizeX << ht << board.sizeY << lf;
	// TODO: EntityFactories
	for(size_t x = board.sizeX - 1; x > 0; x--) {
		for(size_t y = board.sizeY - 1; y > 0; y--) {
			auto e = board.getTerrain(x, y);
			*this << *e;
		}
	}
	*this << "T" << lf;
	*this << "Entities";
	board.mapEntities([this](shared_ptr<Entity> e) {*this << *e;});
	send();
	string command;
	while (!(command == "L" || in.eof() || this->owner.willExit())) {
		bool ack = false;
		cerr << endl << ">";
		recv();
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
			*this << this->frame;
			board.mapEntities([this, frame](shared_ptr<Entity> e) {
					if (e->getFrame() > frame) {
					*this << *e;
					}
					});
			for(auto& p : board.getPlayers()) {
				if (p->getFrame() > frame) {
					*this << p->serialize();
				}
			}
			deletedMutex.lock();
			while (deleted.size() > 0) {
				*this << "D\t" << deleted.front() << lf;
				deleted.pop();
			}
			deletedMutex.unlock();
			ack = true;
		}
		if (ack) {
			outBuffer.insert(outBuffer.begin(), ack);
		} else {
			outBuffer.clear();
			outBuffer.push_back(nak);
		}
		send();
	}
}

RemoteClient::~RemoteClient() {
	if(running) {
		th.join();
	}
}

void RemoteClient::recv() {
	inBuffer.clear();
	const size_t bufsize = 4096;
	char b[bufsize];
	long size;
	bool cont = true;
	do {
		memset(b, nul, bufsize);
		size = socket->Recv((void *)b, bufsize-1);
		if(size > 0) {
			cerr << size << " bytes partial: `" << b << '`';
			inBuffer.insert(inBuffer.end(), b, b + size);
			cerr << ", last char is " << (int)b[size - 1];
			cont = b[size - 1] != ht;
		} else {
			cont = false;
			if (size < 0) {
				cerr << "Error in connection!";
			} else {
				cerr << "Empty partial";
			}
		}
		cerr << endl;
	} while (cont);
	size_t s = (size_t)inBuffer.size() + 1;
	char message[s];
	strncpy(message, inBuffer.data(), s - 1);
	message[s - 1] = nul;
	cerr << "Received `" << message << '`' << endl;
}

void RemoteClient::send() {
	outBuffer.push_back(eot);
	socket->Send((void *)outBuffer.data(), outBuffer.size());
	outBuffer.clear();
}

RemoteClient& RemoteClient::operator<<(char c) {
	outBuffer.push_back(c);
	return *this;
}

RemoteClient& RemoteClient::operator<<(int i) {
	return *this<<(long)i;
}

RemoteClient& RemoteClient::operator<<(long l) {
	auto n = htonl(l);
	for(auto i = sizeof(n); i > 0; i--) {
		*this << (char)n;
		n >>= 8;
	}
}

RemoteClient& RemoteClient::operator<<(size_t s) {
	return *this<<(long)s;
}

RemoteClient& RemoteClient::operator<<(std::string s) {
	for(size_t i = 0; i < s.length(); i++) {
		*this<<s[i];
	}
	*this<<nul;
	return *this;
}

RemoteClient& RemoteClient::operator<<(double d) {
	return *this << (int)(d * 100);
}

RemoteClient& RemoteClient::operator<<(r2 r) {
	return *this << r.x << r.y;
}

RemoteClient& RemoteClient::operator<<(Entity& e) {
	*this << e.getId() << e.name
		<< e.getFrame()
		<< e.owner.getId()
		<< e.getPosition()
		<< e.getOrientation();
	return *this;
}

