#ifndef __PARSERYAML_H__
#define __PARSERYAML_H__

#include <iostream>
#include <fstream>
#include "yaml-cpp/yaml.h"
#include "../defines/defines.h"
#include "../log/logger.h"

using namespace std;

struct TagPantalla2{
	unsigned int ancho;
	unsigned int alto;
	unsigned int margen_scroll;
	unsigned int velocidad_scroll;
};

struct TagConfiguracion2{
	unsigned int dt;
};

struct TagTipoEntidad2{
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
	unsigned int sight_radius;
	unsigned int speed;
	bool solid;
	unsigned int capacity;
};

struct TagEntidad2{
	unsigned int pos_x;
	unsigned int pos_y;
	std::string tipoEntidad;
};

struct TagJugador2 {
	std::string name;
	bool isHuman;
	std::vector<TagEntidad2> entidades;
};

struct TagEscenario2{
	std::string nombre;
	unsigned int size_x;
	unsigned int size_y;
	long max_resources;
	std::vector<TagEntidad2> entidades;
	std::vector<TagEntidad2> terrenos;
	std::vector<TagJugador2> jugadores;
};

class ParserYAML
{
private:
	std::string filename;
	YAML::Node doc;

	void setConfiguracion (const YAML::Node& node, TagConfiguracion2& configuracion);
	void setPantalla (const YAML::Node& node, TagPantalla2& pantalla);
	void setTipoEntidad (const YAML::Node& node, TagTipoEntidad2& tipoEntidad, int i);
	void setTipoTerreno (const YAML::Node& node, TagTipoEntidad2& tipoTerreno, int i);
	void setTipoRecurso(const YAML::Node& node, TagTipoEntidad2& tipoTerreno, int i);
	void setEntidad (const YAML::Node& node, TagEntidad2& entidad);
	void setEscenario(const YAML::Node& node, TagEscenario2& escenario);
	void setJugador(const YAML::Node& node, TagJugador2& jugador , int i);
	std::string intToString(int i);
	bool esNumero(std::string s);
	bool obtenerValorScalarNumericoPositivo(const YAML::Node & nodo, std::string tag, unsigned int & salida);
	bool obtenerValorScalarNumericoPositivo(const YAML::Node & nodo, std::string tag, long & salida);
	bool obtenerValorScalarNumericoPositivo(const YAML::Node & nodo, std::string tag, double & salida);
	bool obtenerValorScalarAlfaNumerico(const YAML::Node & nodo, std::string tag, std::string & salida);
	std::string ubicarNodo(const YAML::Mark mark);
	void setArchivoDefault();
	void setConfiguracionDefault(TagConfiguracion2& configuracion);
	void setPantallaDefault (TagPantalla2& pantalla);
	void setTipoEntidadDefault (TagTipoEntidad2& tipoEntidad, int i);
	void setTipoTerrenoDefault(TagTipoEntidad2& tipoTerreno, int i);
	void setTipoRecursoDefault(TagTipoEntidad2& tipoRecurso, int i);
	void setEscenarioDefault (TagEscenario2& escenario);
	void setProtagonistaDefault (TagEntidad2& protagonista);
	
	

public:
	ParserYAML(std::string filename);
	void parse();
	TagConfiguracion2 getConfiguracion();
	TagPantalla2 getPantalla();
	std::vector<TagTipoEntidad2> getTiposEntidades();
	std::vector<TagTipoEntidad2> getTiposTerrenos();
	std::vector<TagTipoEntidad2> getTiposRecursos();
	TagEscenario2 getEscenario();
	~ParserYAML(void);
};

#endif
