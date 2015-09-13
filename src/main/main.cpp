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

	Logger::getInstance()->writeError("Error");
	Logger::getInstance()->writeInformation("Info");
	Logger::getInstance()->writeWarning("Warning");

	{
		GameWindow gameWin = GameWindow();
		gameWin.start();
	}


	Logger::getInstance()->writeInformation("Closing down");

	//
	delete (Logger::getInstance());
	//
	exit(0);
}
//-----------------------------------------------------------------------------
