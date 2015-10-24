#ifndef __GENERICPARSERYAML_H__
#define __GENERICPARSERYAML_H__

#include <iostream>
#include <fstream>
#include "yaml-cpp/yaml.h"
#include "../log/logger.h"

using namespace std;


class GenericParserYAML
{
protected:
	std::string filename;
	std::string filenameDefault;
	YAML::Node doc;
	std::string intToString(int i);
	bool esNumero(std::string s);
	bool obtenerValorScalarNumericoPositivo(const YAML::Node & nodo, std::string tag, unsigned int & salida);
	bool obtenerValorScalarNumericoPositivo(const YAML::Node & nodo, std::string tag, long & salida);
	bool obtenerValorScalarNumericoPositivo(const YAML::Node & nodo, std::string tag, double & salida);
	bool obtenerValorScalarAlfaNumerico(const YAML::Node & nodo, std::string tag, std::string & salida);
	std::string ubicarNodo(const YAML::Mark mark);
	void setArchivoDefault();

public:
	GenericParserYAML(std::string filename, std::string filenameDefault);
	void parse();
	~GenericParserYAML(void);
};

#endif