#include <sstream>

#include "remote_board.h"

#include "../model/board.h"
#include "../model/command.h"
#include "../model/charnames.h"
#include "../model/entity.h"
#include "../model/entity_factory.h"

#include "../parser_yaml/client_parser.h"
#include "../parser_yaml/ruleset_parser.h"

using namespace std;
using namespace charnames;

RemoteBoard::RemoteBoard(Game& game, RulesetParser& rulesetParser, ClientParser& clientParser, GameModes gameMode) :
	ABoard(game, rulesetParser,
			"Loading...",
			0, 0, 0, gameMode)
{
	stringstream message;
	message << "Creating RemoteBoard " << this;
	Logger::getInstance()->writeInformation(message.str());

	socket = Socket::create();
	auto conf = clientParser.getClientConfiguracion();
	if (!socket->Connect(conf.address, conf.port)) {
		Logger::getInstance()->writeError("Could not connect!");
		return;
	}
	if(!socket->flushIn()) {
		Logger::getInstance()->writeError("Could not connect!");
		return;
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
			}
		}
		// Relleno con TERRENO_DEFAULT
		fillTerrain();
		{
			// Entities
			char c = nul;
			*socket >> c;
			while(c == gs) {
				readEntity();
				*socket >> c;
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
	if(!isRunning()) {
		return;
	}
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
	if(!flushOut()) {
		Logger::getInstance()->writeError("Lost connection to server!");
		state = BoardState::error;
		return;
	}
	char ackSink = nul;
	*socket >> ackSink;
	if(ackSink != ack) {
		return;
	}
	*socket >> frame;
	char next = nul;
	do {
		*socket >> next;
		switch (next) {
			case 'D':
				{
					size_t id;
					*socket >> id;
					auto e = findEntity(id);
					if(e) {
						e->setDeletable();
					}
				}
				break;
			case 'E':
				{
					readEntity();
				}
				break;
			case 'L':
				{
					state = BoardState::finished;
					next = eot;
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

void RemoteBoard::updateResources(string playerName) {
	auto& p = findPlayer(playerName);
	// Activity
	bool active = false, alive = true;
	*socket >> active >> alive;
	p.setActive(active);
	if (!alive) {
		p.kill();
	}
	// Resources
	char c = nul;
	*socket >> c;
	while(c == gs) {
		c = nul;
		string resName = "";
		long resAmount = 0;
		*socket >> resName >> resAmount >> c;
		p.setResources(resName, resAmount);
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

void RemoteBoard::execute(BuildCommand& command) {
	*socket << 'B' << command.entityId << command.position << command.entityType;
	flushOut();
}

void RemoteBoard::execute(CreateCommand& command) {
	*socket << 'C' << command.entityId << command.entityType;
	flushOut();
}

void RemoteBoard::execute(GatherCommand& command) {
	*socket << 'G' << command.entityId << command.targetId;
	flushOut();
}

void RemoteBoard::execute(AttackCommand& command) {
	*socket << 'A' << command.entityId << command.targetId;
	flushOut();
}

void RemoteBoard::execute(RepairCommand& command) {
	*socket << 'R' << command.entityId << command.targetId;
	flushOut();
}

bool RemoteBoard::flushOut() {
	if(!socket->flushOut()) {
		return false;
	}
	return true;
}

void RemoteBoard::readEntity() {
	char t = nul;
	size_t id, f;
	string ename, owner, product;
	r2 pos;
	double orientation = 0;
	int health = 0;
	int progress = 0;
	int cargo = 0;
	bool isInAction = false;
	*socket >> t >> id >> ename >> owner >> f >> pos.x >> pos.y;
	switch (t) {
		case 'B':
			{
				*socket >> health >> isInAction >> progress >> product;
				break;
			}
		case 'U':
			{
				*socket >> health >> progress;
				break;
			}
		case 'R':
			{
				*socket >> cargo;
				break;
			}
		case 'N':
			{
				*socket >> health >> isInAction >> orientation;
				break;
			}
		case 'F':
			{
				*socket >> health;
				break;
			}
	}
	auto e = findEntity(id);
	if (!e) {
		if (t == 'U') {
			auto entityFactory = entityFactories[ename];
			auto entityFactoryUnfinished = dynamic_cast<BuildingFactory*>(entityFactory.get());
			if (entityFactoryUnfinished && players[owner]) {
				e = entityFactoryUnfinished->createUnfinished(*(players[owner]), pos);
				createEntity(e);
			}
		} else {
			e = createEntity(ename, owner, pos);
		}
	}
	if (e) {
		e->setId(id);
		e->setFrame(f);
		e->setPosition(pos);
		e->isInAction = isInAction;
	}
	auto p = e.get();
	if(auto u = dynamic_cast<UnfinishedBuilding*>(p)) {
		u->health.set(health);
		u->progress.set(progress);
	} else if(auto b = dynamic_cast<Building*>(p)) {
		b->health.set(health);
		b->progress.set(progress);
		b->currentProduct = product;
	} else if(auto r = dynamic_cast<Resource*>(p)) {
		r->cargo.set(cargo);
	} else if(auto n = dynamic_cast<Unit*>(p)) {
		n->health.set(health);
		n->setOrientation(orientation);
	} else if(auto f = dynamic_cast<Flag*>(p)) {
		f->health.set(health);
	}
}

