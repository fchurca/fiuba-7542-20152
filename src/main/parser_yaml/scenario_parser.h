#ifndef __SCENARIOPARSERYAML_H__
#define __SCENARIOPARSERYAML_H__

#include <iostream>
#include <fstream>
#include "yaml-cpp/yaml.h"
#include "../defines/defines.h"
#include "../log/logger.h"
#include "generic_parser.h"

using namespace std;

struct TagEntidad {
	unsigned int pos_x;
	unsigned int pos_y;
	std::string tipoEntidad;
};

struct TagJugador {
	std::string name;
	bool isHuman;
	std::vector<TagEntidad> entidades;
};

struct TagEscenario {
	std::string nombre;
	unsigned int size_x;
	unsigned int size_y;
	long max_resources;
	std::vector<TagEntidad> entidades;
	std::vector<TagEntidad> terrenos;
	std::vector<TagJugador> jugadores;
};

class ScenarioParserYAML : GenericParserYAML
{
private:
	void setEntidad(const YAML::Node& node, TagEntidad& entidad);
	void setEscenario(const YAML::Node& node, TagEscenario& escenario);
	void setJugador(const YAML::Node& node, TagJugador& jugador, int i);
	void setEscenarioDefault(TagEscenario& escenario);
	void setProtagonistaDefault(TagEntidad& protagonista);
public:
	ScenarioParserYAML(std::string filename, std::string filenameDefault);
	TagEscenario getEscenario();
	~ScenarioParserYAML(void);
};

#endif