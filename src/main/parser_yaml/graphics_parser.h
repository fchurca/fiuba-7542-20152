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
	unsigned int minimapa_ancho;
	unsigned int hud_alto;
	unsigned int margen_scroll;
	unsigned int velocidad_scroll;
	unsigned int size_text;
};

class GraphicsParser : public GenericParser
{
private:
	void setPantalla(const YAML::Node& node, TagPantalla& pantalla);
	void setPantallaDefault(TagPantalla& pantalla);

public:
	GraphicsParser(std::string filename);
	TagPantalla getPantalla();
	~GraphicsParser(void);
};

#endif
