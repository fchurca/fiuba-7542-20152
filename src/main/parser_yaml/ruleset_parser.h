#ifndef __RULESETPARSER_H__
#define __RULESETPARSER_H__

#include <fstream>
#include "yaml-cpp/yaml.h"
#include "generic_parser.h"
#include "../defines/defines.h"
#include "../log/logger.h"

struct TagCosto {
	std::string recurso;
	unsigned int cantidad;
};

struct TagProduct {
	std::string nombre;
	std::vector<TagCosto> costos;
};

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
	std::string behaviour;
	unsigned int health;
	unsigned int armour;
	std::vector<TagProduct> products;
	std::string resource_name;
	unsigned int hit_force;
	unsigned int hit_radius;
};

struct TagConfiguracion {
	unsigned int dt;
};

class RulesetParser : public GenericParser
{
private:
	void setConfiguracion(const YAML::Node& node, TagConfiguracion& configuracion);
	void setConfiguracionDefault(TagConfiguracion& configuracion);
	void setTipoUnidad(const YAML::Node& node, TagTipoEntidad& tipoEntidad, int i);
	void setTipoEstructura(const YAML::Node& node, TagTipoEntidad& tipoEntidad, int i);
	void setTipoTerreno(const YAML::Node& node, TagTipoEntidad& tipoTerreno, int i);
	void setTipoRecurso(const YAML::Node& node, TagTipoEntidad& tipoTerreno, int i);
	void setProduct(const YAML::Node& node, TagProduct& tipoEntidad, int i);
	void setTipoUnidadDefault(TagTipoEntidad& tipoEntidad, int i);
	void setTipoTerrenoDefault(TagTipoEntidad& tipoTerreno, int i);
	void setTipoRecursoDefault(TagTipoEntidad& tipoRecurso, int i);
	void setTipoEstructuraDefault(TagTipoEntidad& tipoEntidad, int i);
	std::vector<TagTipoEntidad> getTiposUnidadesInternal();
	std::vector<TagTipoEntidad> getTiposEstructurasInternal();
public:
	RulesetParser(std::string filename);
	TagConfiguracion getConfiguracion();
	std::vector<TagTipoEntidad> getTiposUnidades();
	std::vector<TagTipoEntidad> getTiposEstructuras();
	std::vector<TagTipoEntidad> getTiposTerrenos();
	std::vector<TagTipoEntidad> getTiposRecursos();
	~RulesetParser(void);
};

#endif
