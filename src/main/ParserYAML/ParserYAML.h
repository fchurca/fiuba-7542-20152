#ifndef __PARSERYAML_H__
#define __PARSERYAML_H__

#include <iostream>
#include <fstream>
#include "yaml-cpp/yaml.h"

using namespace std;

struct TagPantalla{
	unsigned int ancho;
	unsigned int alto;
};

struct TagConfiguracion{
	unsigned int vel_personaje;
	unsigned int margen_scroll;
};

struct TagTipoEntidad{
	std::string nombre;
	std::string imagen;
	unsigned int ancho_base;
	unsigned int alto_base;
	unsigned int pixel_ref_x;
	unsigned int pixel_ref_y;
    unsigned int fps;
	unsigned int delay;
};

struct TagEntidad{
	unsigned int pos_x;
	unsigned int pos_y;
	std::string tipoEntidad;
};

struct TagEscenario{
	std::string nombre;
	unsigned int size_x;
	unsigned int size_y;
	std::vector<TagEntidad> entidades;
	TagEntidad protagonista;
};

class ParserYAML
{
private:
	std::string filename;
	YAML::Node doc;

	void setConfiguracion (const YAML::Node& node, TagConfiguracion& configuracion);
	void setPantalla (const YAML::Node& node, TagPantalla& pantalla);
	void setTipoEntidad (const YAML::Node& node, TagTipoEntidad& tipoEntidad);
	void setEntidad (const YAML::Node& node, TagEntidad& entidad);
	void setEscenario(const YAML::Node& node, TagEscenario& escenario);
	

public:
	ParserYAML(std::string filename);
	void parse();
	TagConfiguracion getConfiguracion();
	TagPantalla getPantalla();
	std::vector<TagTipoEntidad> getTiposEntidades();
	TagEscenario getEscenario();

	bool ParserYAML::esNumero(std::string s);
	~ParserYAML(void);
};

#endif