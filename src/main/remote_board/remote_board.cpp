#include <sstream>

#include "remote_board.h"

#include "../model/board.h"
#include "../model/command.h"
#include "../model/entity.h"
#include "../parser_yaml/ruleset_parser.h"

using namespace std;

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
	socket->Recv(nullptr, 0);
	socket->Send("L", 1);

	// Relleno con TERRENO_DEFAULT
	for(size_t x = 0; x < sizeX; x++) {
		for(size_t y = 0; y < sizeY; y++) {
			if (!getTerrain(x, y)) {
				setTerrain(TERRENO_DEFAULT_NOMBRE, x, y);
			}
		}
	}
}

RemoteBoard::~RemoteBoard() {
	stringstream message;
	message << "Killing RemoteBoard " << this;
	Logger::getInstance()->writeInformation(message.str());
}

void RemoteBoard::update() {
	ABoard::update();
	for(auto& e : entities) {
		e->update();
	}
}

void RemoteBoard::execute(StopCommand& command) {
}

void RemoteBoard::execute(MoveCommand& command) {
}

