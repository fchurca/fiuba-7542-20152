#include <iostream>
#include <fstream>
#include <string>
#include <vector>
//-----------------------------------------------------------------------------
#include <SDL2/SDL.h>
//-----------------------------------------------------------------------------
#include "parser_yaml/parser_yaml.h"
#include "log/logger.h"
#include "gfx/game_window.h"
//-----------------------------------------------------------------------------
#define CONFIG_FILE_PATH "config.yaml"
//-----------------------------------------------------------------------------
int main(int argc, char* args[]) {

	Logger::getInstance()->writeError("Error");
	Logger::getInstance()->writeInformation("Info");
	Logger::getInstance()->writeWarning("Warning");

	Logger::getInstance()->writeInformation("Creating window");
	GameWindow window = GameWindow();

	for(bool quit = false; !quit;) {
		for(SDL_Event e; SDL_PollEvent(&e) != 0;) {
			quit |= e.type == SDL_QUIT;
			Logger::getInstance()->writeInformation("Event received");
		}
	}

	//	test parser
	ParserYAML* parser = new ParserYAML(CONFIG_FILE_PATH);
	parser->parse();
	TagPantalla tp = parser->getPantalla();
	TagConfiguracion tc = parser->getConfiguracion();
	std::vector<TagTipoEntidad> tte = parser->getTiposEntidades();
	TagEscenario te = parser->getEscenario();

	Logger::getInstance()->writeInformation("Closing down");
	delete parser;
	//
	exit(0);
}
//-----------------------------------------------------------------------------
