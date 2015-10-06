#ifndef __PARSERYAML_H__
#define __PARSERYAML_H__

#include <iostream>
#include <fstream>
#include "yaml-cpp/yaml.h"

#include "server_defines.h"
//#include "../log/logger.h"

using namespace std;

struct TagServerConfiguration {
	std::string ip;
	unsigned int port;
	unsigned int max_clients;
};

struct TagServerPaths {
	std::string original_path;
	std::string default_path;
};

class ServerParserYAML
{
private:
	std::string filename;
	YAML::Node doc;

	std::string intToString(int i);
	bool esNumero(std::string s);
	bool obtenerValorScalarNumericoPositivo(const YAML::Node & nodo, std::string tag, unsigned int & salida);
	bool obtenerValorScalarAlfaNumerico(const YAML::Node & nodo, std::string tag, std::string & salida);
	std::string ubicarNodo(const YAML::Mark mark);
	void setServerConfiguration(const YAML::Node& node, TagServerConfiguration& configuration);
	void setServerConfigurationDefault(TagServerConfiguration& configuration);
	void setServerPaths(const YAML::Node& node, TagServerPaths& paths);
	void setServerPathsDefault(TagServerPaths& paths);
	void setArchivoDefault();


public:
	ServerParserYAML(std::string filename);
	void parse();
	TagServerConfiguration getServerConfiguration();
	TagServerPaths getServerPaths();
};

#endif