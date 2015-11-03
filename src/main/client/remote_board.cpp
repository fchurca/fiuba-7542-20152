#include <sstream>

#include "remote_board.h"

#include "../model/board.h"
#include "../model/command.h"
#include "../model/charnames.h"
#include "../model/entity.h"

#include "../parser_yaml/client_parser.h"
#include "../parser_yaml/ruleset_parser.h"

using namespace std;
using namespace charnames;

#include <iostream>

RemoteBoard::RemoteBoard(RulesetParser& rulesetParser, ClientParser& clientParser) :
	ABoard(rulesetParser,
			"Loading...",
			0, 0, 0)
{
	stringstream message;
	message << "Creating RemoteBoard " << this;
	Logger::getInstance()->writeInformation(message.str());

	socket = Socket::create();
	auto conf = clientParser.getClientConfiguracion();
	if (!socket->Connect(conf.address, conf.port)) {
		cerr << "Could not connect!" << endl;
	}
	if(!socket->flushIn()) {
		cerr << "Could not connect!" << endl;
	}
	char c = nul;
	*socket >> c;
	if (c == ack) {
		*socket >> name >> sizeX >> sizeY >> frame >> maxResources;
		terrain.resize(sizeX * sizeY);
		string pname;
		*socket >> pname;
		createPlayer(pname, true);
		// Assigned player
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
	flushOut();
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
	if(flushOut()) {
		char ackSink = nul;
		*socket >> ackSink;
		if(ackSink == ack) {
			*socket >> frame;
			char next = nul;
			do {
				*socket >> next;
				switch (next) {
					case 'E':
						{
							size_t id, f;
							string ename, owner;
							r2 pos;
							double orientation;
							*socket >> id >> ename >> owner >> f >> pos.x >> pos.y >> orientation;
							auto e = findEntity(id);
							e->setFrame(f);
							e->setPosition(pos);
							e->setOrientation(orientation);
						}
						break;
					case 'D':
						{
							size_t id;
							*socket >> id;
							auto e = findEntity(id);
							e->setDeletable();
						}
						break;
					case 'P':
						{
							string pname;
							*socket >> pname;
							updateResources(pname);
						}
						break;
				}
			} while(next != eot);
		}
	} else {
		cerr << "Server died!" << endl;
	}
}

void RemoteBoard::updateResources(string playerName) {
	// Resources
	char c = nul;
	*socket >> c;
	while(c == gs) {
		c = nul;
		string resName = "";
		long resAmount = 0;
		*socket >> resName >> resAmount >> c;
		cerr << resAmount << endl;
		findPlayer(playerName).setResources(resName, resAmount);
	}
}

void RemoteBoard::execute(StopCommand& command) {
	*socket << 'S' << command.entityId;
	flushOut();
}

void RemoteBoard::execute(MoveCommand& command) {
	*socket << 'M' << command.entityId
		<< command.position.x << command.position.y;
	flushOut();
}

bool RemoteBoard::flushOut() {
	if(!socket->flushOut()) {
		return false;
	}
	return true;
}