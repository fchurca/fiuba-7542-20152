#ifndef __PARSERYAML_H__
#define __PARSERYAML_H__

#include <iostream>
#include <fstream>
#include "yaml-cpp/yaml.h"
#include "../defines/defines.h"
#include "../log/logger.h"

using namespace std;

struct TagPantalla{
	unsigned int ancho;
	unsigned int alto;
};

struct TagConfiguracion{
	unsigned int vel_personaje;
	unsigned int margen_scroll;
	unsigned int velocidad_scroll;
	unsigned int dt;
};

struct TagTipoEntidad{
	std::string nombre;
	std::string imagen;
	double ancho_base;
	double alto_base;
	unsigned int pixel_ref_x;
	unsigned int pixel_ref_y;
	unsigned int alto_sprite;
	unsigned int ancho_sprite;
	unsigned int cantidad_sprites;
    double fps;
	double delay;
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
	std::vector<TagEntidad> terrenos;
	TagEntidad protagonista;
};

class ParserYAML
{
private:
	std::string filename;
	YAML::Node doc;

	void setConfiguracion (const YAML::Node& node, TagConfiguracion& configuracion);
	void setPantalla (const YAML::Node& node, TagPantalla& pantalla);
	void setTipoEntidad (const YAML::Node& node, TagTipoEntidad& tipoEntidad, int i);
	void setTipoTerreno (const YAML::Node& node, TagTipoEntidad& tipoTerreno, int i);
	void setEntidad (const YAML::Node& node, TagEntidad& entidad);
	void setEscenario(const YAML::Node& node, TagEscenario& escenario);
	std::string intToString(int i);
	bool esNumero(std::string s);
	bool validarScalarNumericoPositivo(const YAML::Node & nodo, std::string tag, unsigned int & salida);
	bool validarScalarNumericoPositivo(const YAML::Node & nodo, std::string tag, double & salida);
	bool validarScalarAlfaNumerico(const YAML::Node & nodo, std::string tag, std::string & salida);
	std::string ubicarNodo(const YAML::Mark mark);
	void setArchivoDefault();
	void setConfiguracionDefault(TagConfiguracion& configuracion);
	void setPantallaDefault (TagPantalla& pantalla);
	void setTipoEntidadDefault (TagTipoEntidad& tipoEntidad);
	void setEscenarioDefault (TagEscenario& escenario);
	void setProtagonistaDefault (TagEntidad& protagonista);
	
	

public:
	ParserYAML(std::string filename);
	void parse();
	TagConfiguracion getConfiguracion();
	TagPantalla getPantalla();
	std::vector<TagTipoEntidad> getTiposEntidades();
	std::vector<TagTipoEntidad> getTiposTerrenos();
	TagEscenario getEscenario();
	~ParserYAML(void);
};

#endif
