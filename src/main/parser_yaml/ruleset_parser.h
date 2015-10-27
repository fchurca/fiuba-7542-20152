#ifndef __RULESETPARSER_H__
#define __RULESETPARSER_H__

#include <iostream>
#include <fstream>
#include "yaml-cpp/yaml.h"
#include "generic_parser.h"
#include "../defines/defines.h"
#include "../log/logger.h"

struct TagTipoEntidad {
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

struct TagConfiguracion {
	unsigned int dt;
};

class RulesetParser : public GenericParser
{
private:
	void setConfiguracion(const YAML::Node& node, TagConfiguracion& configuracion);
	void setConfiguracionDefault(TagConfiguracion& configuracion);
	void setTipoEntidad(const YAML::Node& node, TagTipoEntidad& tipoEntidad, int i);
	void setTipoTerreno(const YAML::Node& node, TagTipoEntidad& tipoTerreno, int i);
	void setTipoRecurso(const YAML::Node& node, TagTipoEntidad& tipoTerreno, int i);
	void setTipoEntidadDefault(TagTipoEntidad& tipoEntidad, int i);
	void setTipoTerrenoDefault(TagTipoEntidad& tipoTerreno, int i);
	void setTipoRecursoDefault(TagTipoEntidad& tipoRecurso, int i);
public:
	RulesetParser(std::string filename, std::string filenameDefault);
	TagConfiguracion getConfiguracion();
	std::vector<TagTipoEntidad> getTiposEntidades();
	std::vector<TagTipoEntidad> getTiposTerrenos();
	std::vector<TagTipoEntidad> getTiposRecursos();
	~RulesetParser(void);
};

#endif