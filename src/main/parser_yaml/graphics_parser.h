#ifndef __GRAPHICSPARSER_H__
#define __GRAPHICSPARSER_H__

#include <iostream>
#include <fstream>
#include "yaml-cpp/yaml.h"
#include "generic_parser.h"
#include "../defines/defines.h"
#include "../log/logger.h"

struct TagPantalla {
	unsigned int ancho;
	unsigned int alto;
	unsigned int margen_scroll;
	unsigned int velocidad_scroll;
};

struct TagConfiguracion {
	unsigned int dt;
};

class GraphicsParser : public GenericParser
{
private:
	void setConfiguracion(const YAML::Node& node, TagConfiguracion& configuracion);
	void setPantalla(const YAML::Node& node, TagPantalla& pantalla);
	void setConfiguracionDefault(TagConfiguracion& configuracion);
	void setPantallaDefault(TagPantalla& pantalla);

public:
	GraphicsParser(std::string filename, std::string filenameDefault);
	TagConfiguracion getConfiguracion();
	TagPantalla getPantalla();
	~GraphicsParser(void);
};

#endif
