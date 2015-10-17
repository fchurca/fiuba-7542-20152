#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "../parser_yaml/parser_yaml.h"
#include "../log/logger.h"
#include "../model/game.h"
#include "../gfx/game_window.h"

#include "../remote_client/remote_client.h"

int main(int argc, char* args[]) {

	auto & logger = *Logger::getInstance();
	logger.writeInformation("Start game");

	{
		bool restart = true;
		do {
			Game game;
			ParserYAML parser(CONFIG_FILE_PATH);
			parser.parse();
			game.setBoard(make_shared<Board>(parser));
			game.addClient(make_shared<GameWindow>(game, *(game.getAvailablePlayer()), parser));
			game.addClient(make_shared<RemoteClient>(game, *(game.getBoard()->getPlayers()[1])));
			game.start();
			restart = game.willRestart();
		} while (restart);
	}

	logger.writeInformation("Closing down");

	exit(0);
}

