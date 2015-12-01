#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "../log/logger.h"

#include "../gfx/game_window.h"

#include "../model/game.h"

#include "../parser_yaml/client_parser.h"
#include "../parser_yaml/graphics_parser.h"
#include "../parser_yaml/ruleset_parser.h"
#include "../parser_yaml/scenario_parser.h"

#include "../client/remote_board.h"

#include "../server/server.h"

using namespace std;

int main(int argc, char* argv[]) {

	auto & logger = *Logger::getInstance();
	logger.writeInformation("Start game");

	bool daemon = false, client = false;
	string rulesetFile = RULESET_CONFIG_FILE_PATH;
	string serverFile = SERVER_CONFIG_FILE_PATH;
	string scenarioFile = SCENARIO_CONFIG_FILE_PATH;
	string clientFile = CLIENT_SERVER_CONFIG_FILE_PATH;
	string graphicsFile = GRAPHICS_CONFIG_FILE_PATH;
	GameModes gameMode = NOTHING;

	for(int i = 1; i < argc; i++) {
		switch (argv[i][0]) {
			// Verbosity
			case 'q':
				logger.setLevel(logger.getLevel() - 1);
				break;
			case 'v':
				logger.setLevel(logger.getLevel() + 1);
				break;
			// Client
			case 'C':
				i++;
				if(i < argc) {
					clientFile = argv[i];
				}
			case 'c':
				client = true;
				logger.writeInformation("Starting game as client using " + clientFile);
				break;
			// Daemon
			case 'D':
				i++;
				if(i < argc) {
					serverFile = argv[i];
				}
			case 'd':
				daemon = true;
				logger.writeInformation("Starting game as daemon using " + serverFile);
				break;
			// Graphics
			case 'G':
				i++;
				if(i < argc) {
					graphicsFile = argv[i];
				}
				logger.writeInformation("Using custom graphics " + graphicsFile);
				break;
			// Scenario
			case 'S':
				i++;
				if(i < argc) {
					scenarioFile = argv[i];
				}
				break;
				logger.writeInformation("Using custom scenario " + scenarioFile);
			// Ruleset
			case 'R':
				i++;
				if(i < argc) {
					rulesetFile = argv[i];
				}
				break;
				logger.writeInformation("Using custom ruleset " + rulesetFile);
			//case 'M':
			//	i++;
			//	if (i < argc) {
			//		gameMode = (GameModes)argv[i];
			//	}
			//	break;
			//	logger.writeInformation("Using gameMode " + gameMode);
		}
	}

	if(daemon && client) {
		logger.writeInformation("Starting game as proxy");
	}

	bool restart = true;
	do {
		RulesetParser rulesetParser(rulesetFile);
		rulesetParser.parse();

		Game game;
		shared_ptr<Server> server = nullptr;

		if (client) {
			// Acá estamos levantando el cliente. Lo siguiente en realidad es un RemoteBoard que se conecta por TCP/IP al daemon
			ClientParser clientParser(clientFile);
			clientParser.parse();
			game.setBoard(make_shared<RemoteBoard>(game, rulesetParser, clientParser, gameMode));
		} else {
			ScenarioParser scenarioParser(scenarioFile);
			scenarioParser.parse();
			game.setBoard(make_shared<SmartBoard>(game, rulesetParser, scenarioParser, gameMode));
		}
		if (daemon) {
			ServerParser serverParser(serverFile);
			serverParser.parse();
			server = make_shared<Server>(game, serverParser);
			server->init(); // TODO: Delay until game.start()
		}
		auto graphicPlayer = game.getAvailablePlayer();
		if (graphicPlayer) {
			GraphicsParser graphicsParser(graphicsFile);
			graphicsParser.parse();
			game.addClient(make_shared<GameWindow>(game, *(graphicPlayer), graphicsParser, rulesetParser));
		}
		game.start();
		restart = game.willRestart();
	} while (restart);

	logger.writeInformation("Closing down");

	exit(0);
}

