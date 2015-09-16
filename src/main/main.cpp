#include <iostream>
#include <fstream>
#include <string>
#include <vector>
//-----------------------------------------------------------------------------
#include <SDL2/SDL.h>
//-----------------------------------------------------------------------------
#include "parser_yaml/parser_yaml.h"
#include "log/logger.h"
#include "game.h"
#include "gfx/game_window.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
int main(int argc, char* args[]) {

	auto & logger = *Logger::getInstance();
	logger.writeError("Error");
	logger.getInstance()->writeInformation("Info");
	logger.getInstance()->writeWarning("Warning");

	{
		GameWindow gameWin = GameWindow();
		gameWin.start();
	}

	logger.getInstance()->writeInformation("Closing down");

	exit(0);
}
//-----------------------------------------------------------------------------
