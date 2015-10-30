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

#include "../remote_client/remote_client.h"
#include "../remote_board/remote_board.h"

#include "../server/server.h"

using namespace std;

int main(int argc, char* argv[]) {

	auto & logger = *Logger::getInstance();
	logger.writeInformation("Start game");

	bool standalone = false, daemon = false, client = false;
	if (argc >= 2) {
		switch (argv[1][0]) {
			case 'd': case 'D':
				daemon = true;
				logger.writeInformation("Starting game as daemon");
				break;
			case 'c': case 'C':
				client = true;
				logger.writeInformation("Starting game as client");
				break;
			default:
				standalone = true;
				logger.writeInformation("Starting game as standalone");
				break;
		}
	}

	bool restart = true;
	do {
		RulesetParser rulesetParser(RULESET_CONFIG_FILE_PATH, RULESET_CONFIG_FILE_PATH_DEFAULT);
		rulesetParser.parse();

		Game game;

		ServerParser serverParser(SERVER_CONFIG_FILE_PATH, SERVER_CONFIG_FILE_PATH_DEFAULT);
		serverParser.parse();
		Server server(game, serverParser);
		if (client) {
			// Acá estamos levantando el cliente. Lo siguiente en realidad es un RemoteBoard que se conecta por TCP/IP al daemon
			ClientParser clientParser(CLIENT_SERVER_CONFIG_FILE_PATH, CLIENT_SERVER_CONFIG_FILE_PATH_DEFAULT);
			clientParser.parse();
			game.setBoard(make_shared<RemoteBoard>(rulesetParser, clientParser));
		} else {
			ScenarioParser scenarioParser(SCENARIO_CONFIG_FILE_PATH, SCENARIO_CONFIG_FILE_PATH_DEFAULT);
			scenarioParser.parse();
			game.setBoard(make_shared<SmartBoard>(rulesetParser, scenarioParser));
		}
		if (daemon) {
			server.init();
		}
		auto graphicPlayer = game.getAvailablePlayer();
		if (graphicPlayer) {
			GraphicsParser graphicsParser(GRAPHICS_CONFIG_FILE_PATH, GRAPHICS_CONFIG_FILE_PATH_DEFAULT);
			graphicsParser.parse();
			game.addClient(make_shared<GameWindow>(game, *(graphicPlayer), graphicsParser, rulesetParser));
		}
		game.start();
		restart = game.willRestart();
	} while (restart);

	logger.writeInformation("Closing down");

	exit(0);
}

