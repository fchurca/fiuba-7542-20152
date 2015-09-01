#include <iostream>
#include <fstream>
#include <string>
#include <vector>
//-----------------------------------------------------------------------------
#include "parser_yaml/parser_yaml.h"
//-----------------------------------------------------------------------------
int main(int argc, char* args[]) {

	//	test parser
	ParserYAML* parser = new ParserYAML("configuracion.yaml"); .
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
