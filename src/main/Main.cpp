#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "ParserYAML\ParserYAML.h"
#include "Log\Logger.h"

int main(int argc, char* args[]) {

	Logger::getInstance()->writeError("Error");
	Logger::getInstance()->writeInformation("Info");
	Logger::getInstance()->writeWarning("Warning");
	
	//	test parser
	ParserYAML* parser = new ParserYAML("config.yaml"); //TODO: Actualizar la ruta del archivo.
	parser->parse();
	TagPantalla tp = parser->getPantalla();
	TagConfiguracion tc = parser->getConfiguracion();
	std::vector<TagTipoEntidad> tte = parser->getTiposEntidades();
	TagEscenario te = parser->getEscenario();
	delete parser;
	//
	return 0;
}
//-----------------------------------------------------------------------------
