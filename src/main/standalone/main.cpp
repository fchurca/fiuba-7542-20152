#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "../parser_yaml/parser_yaml.h"
#include "../log/logger.h"
#include "../model/game.h"
#include "../gfx/game_window.h"

#include "../remote_client/remote_client.h"

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
		Game game;
		ParserYAML parser(CONFIG_FILE_PATH);
		parser.parse();
		if (client) {
			// Acá estamos levantando el cliente. Lo siguiente en realidad es un RemoteBoard que se conecta por TCP/IP al daemon
			game.setBoard(make_shared<Board>(parser));
		} else {
			game.setBoard(make_shared<Board>(parser));
		}
		if (daemon) {
			// Acá estamos levantando el server. Todo el siguiente bloque es por cliente nuevo
			auto remotePlayer = game.getAvailablePlayer();
			if (remotePlayer) {
				game.addClient(make_shared<RemoteClient>(game, *(remotePlayer)));
			}
		}
		auto graphicPlayer = game.getAvailablePlayer();
		if (graphicPlayer) {
			game.addClient(make_shared<GameWindow>(game, *(graphicPlayer), parser));
		}
		game.start();
		restart = game.willRestart();
	} while (restart);

	logger.writeInformation("Closing down");

	exit(0);
}

