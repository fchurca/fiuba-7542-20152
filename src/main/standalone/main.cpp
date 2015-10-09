#include <iostream>
#include <fstream>
#include <string>
#include <vector>
//-----------------------------------------------------------------------------
#include <SDL2/SDL.h>
//-----------------------------------------------------------------------------
#include "../parser_yaml/parser_yaml.h"
#include "../log/logger.h"
#include "../model/game.h"
#include "../gfx/game_window.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
int main(int argc, char* args[]) {

	auto & logger = *Logger::getInstance();
	logger.writeInformation("Start game");

	{
		Game game;
		ParserYAML parser(CONFIG_FILE_PATH);
		parser.parse();
		game.setBoard(make_shared<Board>(parser));
		game.addClient(make_shared<GameWindow>(game, *(game.getAvailablePlayer()), parser));
		game.start();
	}

	logger.getInstance()->writeInformation("Closing down");

	exit(0);
}
//-----------------------------------------------------------------------------
