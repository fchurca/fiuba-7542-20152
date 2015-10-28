#include <sstream>

#include "remote_board.h"

#include "../model/board.h"
#include "../model/command.h"
#include "../model/charnames.h"
#include "../model/entity.h"
#include "../parser_yaml/ruleset_parser.h"

using namespace std;
using namespace charnames;

#include <iostream>

RemoteBoard::RemoteBoard(RulesetParser& rulesetParser) :
	ABoard(rulesetParser,
			"Loading...",
			0, 0, 0)
{
	cerr << "Creating RemoteBoard " << this << endl;
	stringstream message;
	message << "Creating RemoteBoard " << this;
	Logger::getInstance()->writeInformation(message.str());

	socket = Socket::create();
	if (!socket->Connect("127.0.0.1", 8001)) {
		cerr << "Could not connect!" << endl;
	}
	if(!socket->flushIn()) {
		cerr << "Could not connect!" << endl;
	}
	char c = nul;
	*socket >> c;
	if (c == ack) {
		*socket >> name >> sizeX >> sizeY >> frame;
		terrain.resize(sizeX * sizeY);
		string pname;
		*socket >> pname;
		createPlayer(pname, true);
		// Assigned player
		cerr << "Player " << pname << ht << endl;
		updateResources(pname);
		// Other players
		{
			char c = nul;
			*socket >> c;
			while(c == gs) {
				c = nul;
				string pname;
				*socket >> pname;
				createPlayer(pname, false);
				cerr << "Player " << pname << ht << endl;
				updateResources(pname);
				*socket >> c;
			}
		}
		// Terrain
		for(size_t x = 0; x < sizeX; x++) {
			for(size_t y = 0; y < sizeY; y++) {
				string tname = "";
				*socket >> tname;
				setTerrain(tname, x, y);
				if (!getTerrain(x, y)) {
					cerr << x << ',' << y << ':' << tname << ht;
				}
			}
		}
		cerr << endl;
		// Relleno con TERRENO_DEFAULT (TODO: deduplicate)
		for(size_t x = 0; x < sizeX; x++) {
			for(size_t y = 0; y < sizeY; y++) {
				if (!getTerrain(x, y)) {
					setTerrain(TERRENO_DEFAULT_NOMBRE, x, y);
				}
			}
		}
		{
			// Entities
			char c = nul;
			*socket >> c;
			while(c == gs) {
				c = nul;
				size_t id, f;
				string ename, owner;
				r2 pos;
				double orientation;
				*socket >> id >> ename >> owner >> f >> pos.x >> pos.y >> orientation >> c;
				auto e = createEntity(ename, owner, pos);
				e->setFrame(f);
				e->setId(id);
				e->setOrientation(orientation);
			}
		}

	}
}

RemoteBoard::~RemoteBoard() {
	*socket << 'L';
	socket->flushOut();
	stringstream message;
	message << "Killing RemoteBoard " << this;
	Logger::getInstance()->writeInformation(message.str());
}

void RemoteBoard::update() {
	ABoard::update();
	for(auto& p : players) {
		if(p.second->human) {
			p.second->update();
		}
	}
	for(auto& e : entities) {
		e->update();
	}
	*socket << 'U' << frame;
	socket->flushOut();
	char ackSink = nul, eotSink = nul;
	*socket >> ackSink;
	if(ackSink == ack) {
		*socket >> frame >> eotSink;
		if(eotSink == eot) {
			cerr << "We are now at frame " << frame;
		}
	} else {
		cerr << "Could not update frame!" << endl;
	}
}

void RemoteBoard::updateResources(string playerName) {
	// TODO: deduplicate
	// Resources
	cerr << "Resources for " << playerName << endl;
	char c = nul;
	*socket >> c;
	while(c == gs) {
		c = nul;
		string resName = "";
		long resAmount = 0;
		*socket >> resName >> resAmount >> c;
		findPlayer(playerName).grantResources(resName, resAmount);
		cerr << "`" << resName << "`:" << resAmount << endl;
	}
	cerr << endl;
}

void RemoteBoard::execute(StopCommand& command) {
}

void RemoteBoard::execute(MoveCommand& command) {
}

