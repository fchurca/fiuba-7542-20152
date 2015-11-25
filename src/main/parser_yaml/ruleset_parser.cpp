#include "ruleset_parser.h"

using namespace std;

RulesetParser::RulesetParser(std::string filename) : GenericParser(filename, RULESET_CONFIG_FILE_PATH_DEFAULT) {
}

RulesetParser::~RulesetParser(void) {

}

TagConfiguracion RulesetParser::getConfiguracion() {
	Logger::getInstance()->writeInformation("YAML-CPP:Se obtiene la informacion de configuracion.");
	TagConfiguracion configuracion;
	if (this->doc.FindValue("configuracion")) {
		const YAML::Node& conf = this->doc["configuracion"];
		if (conf.Type() == YAML::NodeType::Map) {
			setConfiguracion(conf, configuracion);
		}
		else {
			Logger::getInstance()->writeWarning("YAML-CPP:El tag de configuracion no es del tipo Map. Ubicar" + ubicarNodo(conf.GetMark()));
			setConfiguracionDefault(configuracion);
		}
	}
	else {
		Logger::getInstance()->writeWarning("YAML-CPP:El tag de configuracion no existe en el archivo.");
		setConfiguracionDefault(configuracion);
	}
	return configuracion;
}

void RulesetParser::setConfiguracion(const YAML::Node& node, TagConfiguracion& configuracion) {
	if (node.Type() == YAML::NodeType::Map) {
		if (!obtenerValorScalarNumericoPositivo(node, "dt", configuracion.dt)) {
			Logger::getInstance()->writeWarning("YAML-CPP: Se toma por default (dt).");
			configuracion.dt = DT_DEFAULT;
		}
	}
	else {
		Logger::getInstance()->writeWarning("YAML-CPP:El contenido del tag de configuracion no es del tipo Map. Ubicar" + ubicarNodo(node.GetMark()));
		setConfiguracionDefault(configuracion);
	}
}

void RulesetParser::setConfiguracionDefault(TagConfiguracion& configuracion) {
	Logger::getInstance()->writeWarning("YAML-CPP:Se toma configuracion por default.");
	configuracion.dt = DT_DEFAULT;
}

std::vector<TagTipoEntidad> RulesetParser::getTiposUnidades() {
	std::vector<TagTipoEntidad> unidades = getTiposUnidadesInternal();
	//VALIDAMOS PRODUCTS DE LAS UNIDADES
	std::vector<TagTipoEntidad> estructuras = getTiposEstructurasInternal();
	std::vector<TagTipoEntidad> recursos = getTiposRecursos();
	int counterUnidad, counterProduct, counterRecurso;
	bool exist;
	counterUnidad = 0;
	for (auto u : unidades) {
		counterProduct = 0;
		std::vector<TagProduct> products = u.products;
		for (auto p : products) {
			exist = false;
			for (auto e : estructuras) {
				if (e.nombre == p.nombre) {
					exist = true;
					break;
				}
			}
			if (!exist)
				unidades.at(counterUnidad).products.erase(unidades.at(counterUnidad).products.begin() + counterProduct);
			else {
				counterRecurso = 0;
				std::vector<TagCosto> costos = p.costos;
				for (auto c : costos) {
					exist = false;
					for (auto r : recursos) {
						if (r.resource_name == c.recurso) {
							exist = true;
							break;
						}
					}
					if (!exist)
						unidades.at(counterUnidad).products.at(counterProduct).costos.erase(unidades.at(counterUnidad).products.at(counterProduct).costos.begin() + counterRecurso);
					else
						counterRecurso++;
				}
				counterProduct++;
			}	
		}
		counterUnidad++;
	}
	return unidades;
}

std::vector<TagTipoEntidad> RulesetParser::getTiposUnidadesInternal() {
	Logger::getInstance()->writeInformation("YAML-CPP:Se obtiene informacion de los tipos de unidades.");
	std::vector<TagTipoEntidad> tiposDeEntidades;
	if (this->doc.FindValue("tipos_unidades")) {
		const YAML::Node& tipos = this->doc["tipos_unidades"];
		if (tipos.Type() == YAML::NodeType::Sequence) {
			for (unsigned int i = 0; i < tipos.size(); i++) {
				Logger::getInstance()->writeInformation("YAML-CPP:Se obtiene informacion del tipo de unidad numero." + intToString(i));
				TagTipoEntidad tipoEntidad;
				setTipoUnidad(tipos[i], tipoEntidad, i);
				tiposDeEntidades.push_back(tipoEntidad);
			}
		}
		else {
			Logger::getInstance()->writeWarning("YAML-CPP:El tag de tipos de unidad no es del tipo Sequence. Ubicar" + ubicarNodo(tipos.GetMark()));
			Logger::getInstance()->writeInformation("YAML-CPP:No se toman tipos de unidad");
		}
	}
	else {
		Logger::getInstance()->writeWarning("YAML-CPP:El tag de tipos de unidades no existe en el archivo.");
		Logger::getInstance()->writeInformation("YAML-CPP:No se toman tipos de unidades");
	}
	return tiposDeEntidades;
}

std::vector<TagTipoEntidad> RulesetParser::getTiposEstructuras() {
	std::vector<TagTipoEntidad> estructuras = getTiposEstructurasInternal();
	//VALIDAMOS PRODUCTS DE LAS ESTRUCTURAS
	std::vector<TagTipoEntidad> unidades = getTiposUnidadesInternal();
	std::vector<TagTipoEntidad> recursos = getTiposRecursos();
	int counterEstructura, counterProduct, counterRecurso;
	bool exist;
	counterEstructura = 0;
	for (auto e : estructuras) {
		counterProduct = 0;
		std::vector<TagProduct> products = e.products;
		for (auto p : products) {
			exist = false;
			for (auto u : unidades) {
				if (u.nombre == p.nombre) {
					exist = true;
					break;
				}
			}
			if (!exist)
				estructuras.at(counterEstructura).products.erase(estructuras.at(counterEstructura).products.begin() + counterProduct);
			else {
				counterRecurso = 0;
				std::vector<TagCosto> costos = p.costos;
				for (auto c : costos) {
					exist = false;
					for (auto r : recursos) {
						if (r.resource_name == c.recurso) {
							exist = true;
							break;
						}
					}
					if (!exist)
						estructuras.at(counterEstructura).products.at(counterProduct).costos.erase(estructuras.at(counterEstructura).products.at(counterProduct).costos.begin() + counterRecurso);
					else
						counterRecurso++;
				}
				counterProduct++;
			}	
		}
		counterEstructura++;
	}
	return estructuras;
}

std::vector<TagTipoEntidad> RulesetParser::getTiposEstructurasInternal() {
	Logger::getInstance()->writeInformation("YAML-CPP:Se obtiene informacion de los tipos de estructuras.");
	std::vector<TagTipoEntidad> tiposDeEntidades;
	if (this->doc.FindValue("tipos_estructuras")) {
		const YAML::Node& tipos = this->doc["tipos_estructuras"];
		if (tipos.Type() == YAML::NodeType::Sequence) {
			for (unsigned int i = 0; i < tipos.size(); i++) {
				Logger::getInstance()->writeInformation("YAML-CPP:Se obtiene informacion del tipo de estructura numero." + intToString(i));
				TagTipoEntidad tipoEntidad;
				setTipoEstructura(tipos[i], tipoEntidad, i);
				tiposDeEntidades.push_back(tipoEntidad);
			}
		}
		else {
			Logger::getInstance()->writeWarning("YAML-CPP:El tag de tipos de estructura no es del tipo Sequence. Ubicar" + ubicarNodo(tipos.GetMark()));
			Logger::getInstance()->writeInformation("YAML-CPP:No se toman tipos de estructura");
		}
	}
	else {
		Logger::getInstance()->writeWarning("YAML-CPP:El tag de tipos de estructuras no existe en el archivo.");
		Logger::getInstance()->writeInformation("YAML-CPP:No se toman tipos de estructuras");
	}
	return tiposDeEntidades;
}

std::vector<TagTipoEntidad> RulesetParser::getTiposTerrenos() {
	Logger::getInstance()->writeInformation("YAML-CPP:Se obtiene informacion de los tipos de terrenos.");
	std::vector<TagTipoEntidad> tiposDeTerrenos;
	if (this->doc.FindValue("tipos_terrenos")) {
		const YAML::Node& tipos = this->doc["tipos_terrenos"];
		if (tipos.Type() == YAML::NodeType::Sequence) {
			for (unsigned int i = 0; i < tipos.size(); i++) {
				Logger::getInstance()->writeInformation("YAML-CPP:Se obtiene informacion del tipo de terreno numero." + intToString(i));
				TagTipoEntidad tipoTerreno;
				setTipoTerreno(tipos[i], tipoTerreno, i);
				tiposDeTerrenos.push_back(tipoTerreno);
			}
		}
		else {
			Logger::getInstance()->writeWarning("YAML-CPP:El tag de tipos de terrenos no es del tipo Sequence. Ubicar" + ubicarNodo(tipos.GetMark()));
			Logger::getInstance()->writeInformation("YAML-CPP:No se toman tipos de terreno");
		}
	}
	else {
		Logger::getInstance()->writeWarning("YAML-CPP:El tag de tipos de terrenos no existe en el archivo.");
		Logger::getInstance()->writeInformation("YAML-CPP:No se toman tipos de terreno");
	}
	return tiposDeTerrenos;
}
std::vector<TagTipoEntidad> RulesetParser::getTiposRecursos() {
	Logger::getInstance()->writeInformation("YAML-CPP:Se obtiene informacion de los tipos de recursos.");
	std::vector<TagTipoEntidad> tiposDeRecursos;
	if (this->doc.FindValue("tipos_recursos")) {
		const YAML::Node& tipos = this->doc["tipos_recursos"];
		if (tipos.Type() == YAML::NodeType::Sequence) {
			for (unsigned int i = 0; i < tipos.size(); i++) {
				Logger::getInstance()->writeInformation("YAML-CPP:Se obtiene informacion del tipo de recurso numero." + intToString(i));
				TagTipoEntidad tipoRecurso;
				setTipoRecurso(tipos[i], tipoRecurso, i);
				tiposDeRecursos.push_back(tipoRecurso);
			}
		}
		else {
			Logger::getInstance()->writeWarning("YAML-CPP:El tag de tipos de recursos no es del tipo Sequence. Ubicar" + ubicarNodo(tipos.GetMark()));
			Logger::getInstance()->writeInformation("YAML-CPP:No se toman tipos de recurso");
		}
	}
	else {
		Logger::getInstance()->writeWarning("YAML-CPP:El tag de tipos de recursos no existe en el archivo.");
		Logger::getInstance()->writeInformation("YAML-CPP:No se toman tipos de recursos");
	}
	return tiposDeRecursos;
}

void RulesetParser::setTipoUnidad(const YAML::Node& node, TagTipoEntidad& tipoEntidad, int i) {
	if (node.Type() == YAML::NodeType::Map) {
		if (!obtenerValorScalarAlfaNumerico(node, "nombre", tipoEntidad.nombre)) {
			Logger::getInstance()->writeInformation("YAML-CPP:El nombre del tipo de unidad se toma por default.");
			tipoEntidad.nombre = ENTIDAD_DEFAULT_NOMBRE + intToString(i);
		}
		if ((!obtenerValorScalarAlfaNumerico(node, "imagen", tipoEntidad.imagen))
			|| (!obtenerValorScalarNumericoPositivo(node, "pixel_ref_x", tipoEntidad.pixel_ref_x))
			|| (!obtenerValorScalarNumericoPositivo(node, "pixel_ref_y", tipoEntidad.pixel_ref_y))
			|| (!obtenerValorScalarNumericoPositivo(node, "ancho_sprite", tipoEntidad.ancho_sprite))
			|| (!obtenerValorScalarNumericoPositivo(node, "alto_sprite", tipoEntidad.alto_sprite))
			|| (!obtenerValorScalarNumericoPositivo(node, "cantidad_sprites", tipoEntidad.cantidad_sprites))) {
			Logger::getInstance()->writeWarning("YAML-CPP:Datos de la imagen del tipo de unidad invalidos, se toman por default (path, pixel_ref_x, pixel_ref_y, ancho_sprite, alto_sprite, cantidad_sprites).");
			tipoEntidad.imagen = ENTIDAD_DEFAULT_IMAGEN;
			tipoEntidad.pixel_ref_x = ENTIDAD_DEFAULT_PIXEL_REF_X;
			tipoEntidad.pixel_ref_y = ENTIDAD_DEFAULT_PIXEL_REF_Y;
			tipoEntidad.alto_sprite = ENTIDAD_DEFAULT_ALTO_SPRITE;
			tipoEntidad.ancho_sprite = ENTIDAD_DEFAULT_ANCHO_SPRITE;
			tipoEntidad.cantidad_sprites = ENTIDAD_DEFAULT_CANTIDAD_SPRITES;
		}
		if (!obtenerValorScalarNumericoPositivo(node, "ancho_base", tipoEntidad.ancho_base)) {
			Logger::getInstance()->writeWarning("YAML-CPP: Se toma por default (ancho_base).");
			tipoEntidad.ancho_base = ENTIDAD_DEFAULT_ANCHO_BASE;
		}
		if (!obtenerValorScalarNumericoPositivo(node, "alto_base", tipoEntidad.alto_base)) {
			Logger::getInstance()->writeWarning("YAML-CPP: Se toma por default (alto_base).");
			tipoEntidad.alto_base = ENTIDAD_DEFAULT_ALTO_BASE;
		}

		if (!obtenerValorScalarNumericoPositivo(node, "fps", tipoEntidad.fps)) {
			Logger::getInstance()->writeWarning("YAML-CPP: Se toma por default (fps).");
			tipoEntidad.fps = ENTIDAD_DEFAULT_FPS;
		}
		if (!obtenerValorScalarNumericoPositivo(node, "delay", tipoEntidad.delay)) {
			Logger::getInstance()->writeWarning("YAML-CPP: Se toma por default (delay).");
			tipoEntidad.delay = ENTIDAD_DEFAULT_DELAY;
		}
		if (!obtenerValorScalarNumericoPositivo(node, "sight_radius", tipoEntidad.sight_radius)) {
			Logger::getInstance()->writeWarning("YAML-CPP: Se toma por default (sight_radius).");
			tipoEntidad.sight_radius = ENTIDAD_DEFAULT_SIGHT_RADIUS;
		}
		if (!obtenerValorScalarNumericoPositivo(node, "speed", tipoEntidad.speed)) {
			Logger::getInstance()->writeWarning("YAML-CPP: Se toma por default (velocidad personaje).");
			tipoEntidad.speed = ENTIDAD_DEFAULT_SPEED;
		}
		if (!obtenerValorScalarAlfaNumerico(node, "behaviour", tipoEntidad.behaviour)) {
			Logger::getInstance()->writeWarning("YAML-CPP: Se toma por default (dehaviour).");
			tipoEntidad.behaviour = ENTIDAD_DEFAULT_BEHAVIOUR;
		}
		if (!obtenerValorScalarNumericoPositivo(node, "health", tipoEntidad.health)) {
			Logger::getInstance()->writeWarning("YAML-CPP: Se toma por default (health).");
			tipoEntidad.health = ENTIDAD_DEFAULT_HEALTH;
		}
		if (!obtenerValorScalarNumericoPositivo(node, "armour", tipoEntidad.armour)) {
			Logger::getInstance()->writeWarning("YAML-CPP: Se toma por default (armour).");
			tipoEntidad.armour = ENTIDAD_DEFAULT_ARMOUR;
		}
		std::vector<TagProduct> products;
		if (node.FindValue("products")) {
			const YAML::Node& nodeProducts = node["products"];
			if (nodeProducts.Type() == YAML::NodeType::Sequence) {
				for (unsigned int i = 0; i < nodeProducts.size(); i++) {
					TagProduct p;
					setProduct(nodeProducts[i], p, i);
					products.push_back(p);
				}
			}
		}
		tipoEntidad.products = products;
		if (!obtenerValorScalarNumericoPositivo(node, "hit_force", tipoEntidad.hit_force)) {
			Logger::getInstance()->writeWarning("YAML-CPP: Se toma por default (hit_force).");
			tipoEntidad.hit_force = ENTIDAD_DEFAULT_HIT_FORCE;
		}
		if (!obtenerValorScalarNumericoPositivo(node, "hit_radius", tipoEntidad.hit_radius)) {
			Logger::getInstance()->writeWarning("YAML-CPP: Se toma por default (hit_radius).");
			tipoEntidad.hit_radius = ENTIDAD_DEFAULT_HIT_RADIUS;
		}
		//DEFAULT NO USA
		tipoEntidad.solid = true;
		tipoEntidad.capacity = ENTIDAD_DEFAULT_CAPACITY;
		tipoEntidad.resource_name = ENTIDAD_DEFAULT_RESOURCE_NAME;
	}
	else {
		Logger::getInstance()->writeWarning("YAML-CPP:el contenido del tipo de entidad no es del tipo Map. Ubicar" + ubicarNodo(node.GetMark()));
		setTipoUnidadDefault(tipoEntidad, i);
	}
}

void RulesetParser::setTipoEstructura(const YAML::Node& node, TagTipoEntidad& tipoEntidad, int i) {
	if (node.Type() == YAML::NodeType::Map) {
		if (!obtenerValorScalarAlfaNumerico(node, "nombre", tipoEntidad.nombre)) {
			Logger::getInstance()->writeInformation("YAML-CPP:El nombre del tipo de estructura se toma por default.");
			tipoEntidad.nombre = ESTRUCTURA_DEFAULT_NOMBRE + intToString(i);
		}
		if ((!obtenerValorScalarAlfaNumerico(node, "imagen", tipoEntidad.imagen))
			|| (!obtenerValorScalarNumericoPositivo(node, "pixel_ref_x", tipoEntidad.pixel_ref_x))
			|| (!obtenerValorScalarNumericoPositivo(node, "pixel_ref_y", tipoEntidad.pixel_ref_y))
			|| (!obtenerValorScalarNumericoPositivo(node, "ancho_sprite", tipoEntidad.ancho_sprite))
			|| (!obtenerValorScalarNumericoPositivo(node, "alto_sprite", tipoEntidad.alto_sprite))
			|| (!obtenerValorScalarNumericoPositivo(node, "cantidad_sprites", tipoEntidad.cantidad_sprites))) {
			Logger::getInstance()->writeWarning("YAML-CPP:Datos de la imagen del tipo de estructura invalidos, se toman por default (path, pixel_ref_x, pixel_ref_y, ancho_sprite, alto_sprite, cantidad_sprites).");
			tipoEntidad.imagen = ESTRUCTURA_DEFAULT_IMAGEN;
			tipoEntidad.pixel_ref_x = ESTRUCTURA_DEFAULT_PIXEL_REF_X;
			tipoEntidad.pixel_ref_y = ESTRUCTURA_DEFAULT_PIXEL_REF_Y;
			tipoEntidad.alto_sprite = ESTRUCTURA_DEFAULT_ALTO_SPRITE;
			tipoEntidad.ancho_sprite = ESTRUCTURA_DEFAULT_ANCHO_SPRITE;
			tipoEntidad.cantidad_sprites = ESTRUCTURA_DEFAULT_CANTIDAD_SPRITES;
		}
		if (!obtenerValorScalarNumericoPositivo(node, "ancho_base", tipoEntidad.ancho_base)) {
			Logger::getInstance()->writeWarning("YAML-CPP: Se toma por default (ancho_base).");
			tipoEntidad.ancho_base = ESTRUCTURA_DEFAULT_ANCHO_BASE;
		}
		if (!obtenerValorScalarNumericoPositivo(node, "alto_base", tipoEntidad.alto_base)) {
			Logger::getInstance()->writeWarning("YAML-CPP: Se toma por default (alto_base).");
			tipoEntidad.alto_base = ESTRUCTURA_DEFAULT_ALTO_BASE;
		}

		if (!obtenerValorScalarNumericoPositivo(node, "fps", tipoEntidad.fps)) {
			Logger::getInstance()->writeWarning("YAML-CPP: Se toma por default (fps).");
			tipoEntidad.fps = ESTRUCTURA_DEFAULT_FPS;
		}
		if (!obtenerValorScalarNumericoPositivo(node, "delay", tipoEntidad.delay)) {
			Logger::getInstance()->writeWarning("YAML-CPP: Se toma por default (delay).");
			tipoEntidad.delay = ESTRUCTURA_DEFAULT_DELAY;
		}
		if (!obtenerValorScalarNumericoPositivo(node, "sight_radius", tipoEntidad.sight_radius)) {
			Logger::getInstance()->writeWarning("YAML-CPP: Se toma por default (sight_radius).");
			tipoEntidad.sight_radius = ESTRUCTURA_DEFAULT_SIGHT_RADIUS;
		}
		if (!obtenerValorScalarAlfaNumerico(node, "behaviour", tipoEntidad.behaviour)) {
			Logger::getInstance()->writeWarning("YAML-CPP: Se toma por default (dehaviour).");
			tipoEntidad.behaviour = ESTRUCTURA_DEFAULT_BEHAVIOUR;
		}
		if (!obtenerValorScalarNumericoPositivo(node, "health", tipoEntidad.health)) {
			Logger::getInstance()->writeWarning("YAML-CPP: Se toma por default (health).");
			tipoEntidad.health = ESTRUCTURA_DEFAULT_HEALTH;
		}
		if (!obtenerValorScalarNumericoPositivo(node, "armour", tipoEntidad.armour)) {
			Logger::getInstance()->writeWarning("YAML-CPP: Se toma por default (armour).");
			tipoEntidad.armour = ESTRUCTURA_DEFAULT_ARMOUR;
		}
		std::vector<TagProduct> products;
		if (node.FindValue("products")) {
			const YAML::Node& nodeProducts = node["products"];
			if (nodeProducts.Type() == YAML::NodeType::Sequence) {
				for (unsigned int i = 0; i < nodeProducts.size(); i++) {
					TagProduct p;
					setProduct(nodeProducts[i], p, i);
					products.push_back(p);
				}
			}
		}
		tipoEntidad.products = products;
		//DEFAULT NO USA
		tipoEntidad.speed = ESTRUCTURA_DEFAULT_SPEED;
		tipoEntidad.solid = true;
		tipoEntidad.capacity = ESTRUCTURA_DEFAULT_CAPACITY;
		tipoEntidad.resource_name = ESTRUCTURA_DEFAULT_RESOURCE_NAME;
		tipoEntidad.hit_force = ESTRUCTURA_DEFAULT_HIT_FORCE;
		tipoEntidad.hit_radius = ESTRUCTURA_DEFAULT_HIT_RADIUS;
	}
	else {
		Logger::getInstance()->writeWarning("YAML-CPP:el contenido del tipo de entidad no es del tipo Map. Ubicar" + ubicarNodo(node.GetMark()));
		setTipoEstructuraDefault(tipoEntidad, i);
	}
}

void RulesetParser::setTipoTerreno(const YAML::Node& node, TagTipoEntidad& tipoTerreno, int i) {
	if (node.Type() == YAML::NodeType::Map) {
		if (!obtenerValorScalarAlfaNumerico(node, "nombre", tipoTerreno.nombre)) {
			Logger::getInstance()->writeInformation("YAML-CPP: el nombre del tipo de entidad se toma por default.");
			tipoTerreno.nombre = TERRENO_DEFAULT_NOMBRE + intToString(i);
		}
		if ((!obtenerValorScalarAlfaNumerico(node, "imagen", tipoTerreno.imagen))
			|| (!obtenerValorScalarNumericoPositivo(node, "pixel_ref_x", tipoTerreno.pixel_ref_x))
			|| (!obtenerValorScalarNumericoPositivo(node, "pixel_ref_y", tipoTerreno.pixel_ref_y))
			|| (!obtenerValorScalarNumericoPositivo(node, "ancho_sprite", tipoTerreno.ancho_sprite))
			|| (!obtenerValorScalarNumericoPositivo(node, "alto_sprite", tipoTerreno.alto_sprite))
			|| (!obtenerValorScalarNumericoPositivo(node, "cantidad_sprites", tipoTerreno.cantidad_sprites))) {
			Logger::getInstance()->writeWarning("YAML-CPP:Datos de la imagen del tipo de terreno invalidos, se toman por default (path, pixel_ref_x, pixel_ref_y, ancho_sprite, alto_sprite, cantidad_sprites).");
			tipoTerreno.imagen = ENTIDAD_DEFAULT_IMAGEN;
			tipoTerreno.pixel_ref_x = ENTIDAD_DEFAULT_PIXEL_REF_X;
			tipoTerreno.pixel_ref_y = ENTIDAD_DEFAULT_PIXEL_REF_Y;
			tipoTerreno.alto_sprite = ENTIDAD_DEFAULT_ALTO_SPRITE;
			tipoTerreno.ancho_sprite = ENTIDAD_DEFAULT_ANCHO_SPRITE;
			tipoTerreno.cantidad_sprites = ENTIDAD_DEFAULT_CANTIDAD_SPRITES;
		}

		if (!obtenerValorScalarNumericoPositivo(node, "fps", tipoTerreno.fps)) {
			Logger::getInstance()->writeWarning("YAML-CPP: Se toma por default (fps).");
			tipoTerreno.fps = TERRENO_DEFAULT_FPS;
		}
		if (!obtenerValorScalarNumericoPositivo(node, "delay", tipoTerreno.delay)) {
			Logger::getInstance()->writeWarning("YAML-CPP: Se toma por default (delay).");
			tipoTerreno.delay = TERRENO_DEFAULT_DELAY;
		}
		string solid;
		if (!obtenerValorScalarAlfaNumerico(node, "solid", solid) || (solid != "true")) {
			Logger::getInstance()->writeWarning("YAML-CPP: Se toma por default valor false(solid).");
			tipoTerreno.solid = false;
		}
		else
			tipoTerreno.solid = true;
		//DEFAULT no USA
		tipoTerreno.ancho_base = TERRENO_DEFAULT_ANCHO_BASE;
		tipoTerreno.alto_base = TERRENO_DEFAULT_ALTO_BASE;
		tipoTerreno.sight_radius = TERRENO_DEFAULT_SIGHT_RADIUS;
		tipoTerreno.speed = TERRENO_DEFAULT_SPEED;
		tipoTerreno.capacity = TERRENO_DEFAULT_CAPACITY;
		tipoTerreno.behaviour = TERRENO_DEFAULT_BEHAVIOUR;
		tipoTerreno.health = TERRENO_DEFAULT_HEALTH;
		tipoTerreno.armour = TERRENO_DEFAULT_ARMOUR;
		tipoTerreno.products = std::vector<TagProduct>();
		tipoTerreno.resource_name = TERRENO_DEFAULT_RESOURCE_NAME;
		tipoTerreno.hit_force = TERRENO_DEFAULT_HIT_FORCE;
		tipoTerreno.hit_radius = TERRENO_DEFAULT_HIT_RADIUS;
	}
	else {
		Logger::getInstance()->writeWarning("YAML-CPP:El contenido del tipo de terreno ad no es del tipo Map. Ubicar" + ubicarNodo(node.GetMark()));
		setTipoTerrenoDefault(tipoTerreno, i);
	}
}

void RulesetParser::setTipoRecurso(const YAML::Node& node, TagTipoEntidad& tipoRecurso, int i) {
	if (node.Type() == YAML::NodeType::Map) {
		if (!obtenerValorScalarAlfaNumerico(node, "nombre", tipoRecurso.nombre)) {
			Logger::getInstance()->writeInformation("YAML-CPP: el nombre del tipo de recurso se toma por default.");
			tipoRecurso.nombre = RECURSO_DEFAULT_NOMBRE + intToString(i);
		}
		if ((!obtenerValorScalarAlfaNumerico(node, "imagen", tipoRecurso.imagen))
			|| (!obtenerValorScalarNumericoPositivo(node, "pixel_ref_x", tipoRecurso.pixel_ref_x))
			|| (!obtenerValorScalarNumericoPositivo(node, "pixel_ref_y", tipoRecurso.pixel_ref_y))
			|| (!obtenerValorScalarNumericoPositivo(node, "ancho_sprite", tipoRecurso.ancho_sprite))
			|| (!obtenerValorScalarNumericoPositivo(node, "alto_sprite", tipoRecurso.alto_sprite))) {
			Logger::getInstance()->writeWarning("YAML-CPP:Datos de la imagen del tipo de recurso invalidos, se toman por default (path, pixel_ref_x, pixel_ref_y, ancho_sprite, alto_sprite).");
			tipoRecurso.imagen = RECURSO_DEFAULT_IMAGEN;
			tipoRecurso.pixel_ref_x = RECURSO_DEFAULT_PIXEL_REF_X;
			tipoRecurso.pixel_ref_y = RECURSO_DEFAULT_PIXEL_REF_Y;
			tipoRecurso.alto_sprite = RECURSO_DEFAULT_ALTO_SPRITE;
			tipoRecurso.ancho_sprite = RECURSO_DEFAULT_ANCHO_SPRITE;
		}
		if ((!obtenerValorScalarNumericoPositivo(node, "ancho_base", tipoRecurso.ancho_base))
			|| (!obtenerValorScalarNumericoPositivo(node, "alto_base", tipoRecurso.alto_base))) {
			Logger::getInstance()->writeWarning("YAML-CPP:Datos de la imagen del tipo de terreno invalidos, se toman por default (alto_base, ancho_base).");
			tipoRecurso.ancho_base = RECURSO_DEFAULT_ANCHO_BASE;
			tipoRecurso.alto_base = RECURSO_DEFAULT_ALTO_BASE;
		}
		if (!obtenerValorScalarNumericoPositivo(node, "capacidad", tipoRecurso.capacity)) {
			Logger::getInstance()->writeWarning("YAML-CPP: Se toma por default (capacity).");
			tipoRecurso.capacity = RECURSO_DEFAULT_CAPACITY;
		}
		if (!obtenerValorScalarAlfaNumerico(node, "resource_name", tipoRecurso.resource_name)) {
			Logger::getInstance()->writeWarning("YAML-CPP: Se toma por default (resource_name).");
			tipoRecurso.resource_name = RECURSO_DEFAULT_RESOURCE_NAME;
		}
		//DEFAULT NO USA
		tipoRecurso.cantidad_sprites = RECURSO_DEFAULT_CANTIDAD_SPRITES;
		tipoRecurso.fps = RECURSO_DEFAULT_FPS;
		tipoRecurso.delay = RECURSO_DEFAULT_DELAY;
		tipoRecurso.sight_radius = RECURSO_DEFAULT_SIGHT_RADIUS;
		tipoRecurso.speed = RECURSO_DEFAULT_SPEED;
		tipoRecurso.solid = true;
		tipoRecurso.behaviour = RECURSO_DEFAULT_BEHAVIOUR;
		tipoRecurso.health = RECURSO_DEFAULT_HEALTH;
		tipoRecurso.armour = RECURSO_DEFAULT_ARMOUR;
		tipoRecurso.products = std::vector<TagProduct>();
		tipoRecurso.hit_force = RECURSO_DEFAULT_HIT_FORCE;
		tipoRecurso.hit_radius = RECURSO_DEFAULT_HIT_RADIUS;
	}
	else {
		Logger::getInstance()->writeWarning("YAML-CPP:El contenido del tipo de terreno ad no es del tipo Map. Ubicar" + ubicarNodo(node.GetMark()));
		setTipoRecursoDefault(tipoRecurso, i);
	}
}

void RulesetParser::setProduct(const YAML::Node& node, TagProduct& product, int i) {
	if (node.Type() == YAML::NodeType::Map) {
		if (!obtenerValorScalarAlfaNumerico(node, "nombre", product.nombre)) {
			product.nombre = PRODUCT_DEFAULT_NAME + intToString(i);
		}
		std::vector<TagCosto> costos;
		if (node.FindValue("costos")) {
			const YAML::Node& nodeCostos = node["costos"];
			if (nodeCostos.Type() == YAML::NodeType::Sequence) {
				for (unsigned int i = 0; i < nodeCostos.size(); i++) {
					TagCosto c;
					if (!obtenerValorScalarAlfaNumerico(nodeCostos[i], "recurso", c.recurso)) {
						c.recurso = COSTO_DEFAULT_NAME + intToString(i);
					}
					if (!obtenerValorScalarNumericoPositivo(nodeCostos[i], "cantidad", c.cantidad)) {
						c.cantidad = COSTO_DEFAULT_CANTIDAD;
					}
					costos.push_back(c);
				}
			}
			product.costos = costos;
		}
		else {
			product.nombre = PRODUCT_DEFAULT_NAME + intToString(i);
			std::vector<TagCosto> costos;
			product.costos = costos;
		}
	}

}

void RulesetParser::setTipoRecursoDefault(TagTipoEntidad& tipoRecurso, int i) {
	Logger::getInstance()->writeInformation("YAML-CPP:Se toma tipo de recurso por default.");
	tipoRecurso.nombre = RECURSO_DEFAULT_NOMBRE + intToString(i);
	tipoRecurso.imagen = RECURSO_DEFAULT_IMAGEN;
	tipoRecurso.ancho_base = RECURSO_DEFAULT_ANCHO_BASE;
	tipoRecurso.alto_base = RECURSO_DEFAULT_ALTO_BASE;
	tipoRecurso.pixel_ref_x = RECURSO_DEFAULT_PIXEL_REF_X;
	tipoRecurso.pixel_ref_y = RECURSO_DEFAULT_PIXEL_REF_Y;
	tipoRecurso.fps = RECURSO_DEFAULT_FPS;
	tipoRecurso.delay = RECURSO_DEFAULT_DELAY;
	tipoRecurso.alto_sprite = RECURSO_DEFAULT_ALTO_SPRITE;
	tipoRecurso.ancho_sprite = RECURSO_DEFAULT_ANCHO_SPRITE;
	tipoRecurso.cantidad_sprites = RECURSO_DEFAULT_CANTIDAD_SPRITES;
	tipoRecurso.sight_radius = RECURSO_DEFAULT_SIGHT_RADIUS;
	tipoRecurso.speed = RECURSO_DEFAULT_SPEED;
	tipoRecurso.solid = true;
	tipoRecurso.capacity = RECURSO_DEFAULT_CAPACITY;
	tipoRecurso.behaviour = RECURSO_DEFAULT_BEHAVIOUR;
	tipoRecurso.health = RECURSO_DEFAULT_HEALTH;
	tipoRecurso.armour = RECURSO_DEFAULT_ARMOUR;
	tipoRecurso.products = std::vector<TagProduct>();
	tipoRecurso.resource_name = RECURSO_DEFAULT_RESOURCE_NAME;
	tipoRecurso.hit_force = RECURSO_DEFAULT_HIT_FORCE;
	tipoRecurso.hit_radius = RECURSO_DEFAULT_HIT_RADIUS;
}

void RulesetParser::setTipoTerrenoDefault(TagTipoEntidad& tipoEntidad, int i) {
	Logger::getInstance()->writeInformation("YAML-CPP:Se toma tipo de terreno por default.");
	tipoEntidad.nombre = TERRENO_DEFAULT_NOMBRE + intToString(i);
	tipoEntidad.imagen = TERRENO_DEFAULT_IMAGEN;
	tipoEntidad.ancho_base = TERRENO_DEFAULT_ANCHO_BASE;
	tipoEntidad.alto_base = TERRENO_DEFAULT_ALTO_BASE;
	tipoEntidad.pixel_ref_x = TERRENO_DEFAULT_PIXEL_REF_X;
	tipoEntidad.pixel_ref_y = TERRENO_DEFAULT_PIXEL_REF_Y;
	tipoEntidad.fps = TERRENO_DEFAULT_FPS;
	tipoEntidad.delay = TERRENO_DEFAULT_DELAY;
	tipoEntidad.alto_sprite = TERRENO_DEFAULT_ALTO_SPRITE;
	tipoEntidad.ancho_sprite = TERRENO_DEFAULT_ANCHO_SPRITE;
	tipoEntidad.cantidad_sprites = TERRENO_DEFAULT_CANTIDAD_SPRITES;
	tipoEntidad.sight_radius = TERRENO_DEFAULT_SIGHT_RADIUS;
	tipoEntidad.speed = TERRENO_DEFAULT_SPEED;
	tipoEntidad.solid = false;
	tipoEntidad.capacity = TERRENO_DEFAULT_CAPACITY;
	tipoEntidad.behaviour = TERRENO_DEFAULT_BEHAVIOUR;
	tipoEntidad.health = TERRENO_DEFAULT_HEALTH;
	tipoEntidad.armour = TERRENO_DEFAULT_ARMOUR;
	tipoEntidad.products = std::vector<TagProduct>();
	tipoEntidad.resource_name = TERRENO_DEFAULT_RESOURCE_NAME;
	tipoEntidad.hit_force = TERRENO_DEFAULT_HIT_FORCE;
	tipoEntidad.hit_radius = TERRENO_DEFAULT_HIT_RADIUS;
}


void RulesetParser::setTipoUnidadDefault(TagTipoEntidad& tipoEntidad, int i) {
	Logger::getInstance()->writeInformation("YAML-CPP:Se toma tipo de entidad por default.");
	tipoEntidad.nombre = ENTIDAD_DEFAULT_NOMBRE + intToString(i);
	tipoEntidad.imagen = ENTIDAD_DEFAULT_IMAGEN;
	tipoEntidad.ancho_base = ENTIDAD_DEFAULT_ANCHO_BASE;
	tipoEntidad.alto_base = ENTIDAD_DEFAULT_ALTO_BASE;
	tipoEntidad.pixel_ref_x = ENTIDAD_DEFAULT_PIXEL_REF_X;
	tipoEntidad.pixel_ref_y = ENTIDAD_DEFAULT_PIXEL_REF_Y;
	tipoEntidad.fps = ENTIDAD_DEFAULT_FPS;
	tipoEntidad.delay = ENTIDAD_DEFAULT_DELAY;
	tipoEntidad.alto_sprite = ENTIDAD_DEFAULT_ALTO_SPRITE;
	tipoEntidad.ancho_sprite = ENTIDAD_DEFAULT_ANCHO_SPRITE;
	tipoEntidad.cantidad_sprites = ENTIDAD_DEFAULT_CANTIDAD_SPRITES;
	tipoEntidad.sight_radius = ENTIDAD_DEFAULT_SIGHT_RADIUS;
	tipoEntidad.speed = ENTIDAD_DEFAULT_SPEED;
	tipoEntidad.solid = true;
	tipoEntidad.capacity = ENTIDAD_DEFAULT_CAPACITY;
	tipoEntidad.behaviour = ENTIDAD_DEFAULT_BEHAVIOUR;
	tipoEntidad.health = ENTIDAD_DEFAULT_HEALTH;
	tipoEntidad.armour = ENTIDAD_DEFAULT_ARMOUR;
	tipoEntidad.products = std::vector<TagProduct>();
	tipoEntidad.resource_name = ENTIDAD_DEFAULT_RESOURCE_NAME;
	tipoEntidad.hit_force = ENTIDAD_DEFAULT_HIT_FORCE;
	tipoEntidad.hit_radius = ENTIDAD_DEFAULT_HIT_RADIUS;
}

void RulesetParser::setTipoEstructuraDefault(TagTipoEntidad& tipoEntidad, int i) {
	Logger::getInstance()->writeInformation("YAML-CPP:Se toma tipo de entidad por default.");
	tipoEntidad.nombre = ESTRUCTURA_DEFAULT_NOMBRE + intToString(i);
	tipoEntidad.imagen = ESTRUCTURA_DEFAULT_IMAGEN;
	tipoEntidad.ancho_base = ESTRUCTURA_DEFAULT_ANCHO_BASE;
	tipoEntidad.alto_base = ESTRUCTURA_DEFAULT_ALTO_BASE;
	tipoEntidad.pixel_ref_x = ESTRUCTURA_DEFAULT_PIXEL_REF_X;
	tipoEntidad.pixel_ref_y = ESTRUCTURA_DEFAULT_PIXEL_REF_Y;
	tipoEntidad.fps = ESTRUCTURA_DEFAULT_FPS;
	tipoEntidad.delay = ESTRUCTURA_DEFAULT_DELAY;
	tipoEntidad.alto_sprite = ESTRUCTURA_DEFAULT_ALTO_SPRITE;
	tipoEntidad.ancho_sprite = ESTRUCTURA_DEFAULT_ANCHO_SPRITE;
	tipoEntidad.cantidad_sprites = ESTRUCTURA_DEFAULT_CANTIDAD_SPRITES;
	tipoEntidad.sight_radius = ESTRUCTURA_DEFAULT_SIGHT_RADIUS;
	tipoEntidad.speed = ESTRUCTURA_DEFAULT_SPEED;
	tipoEntidad.solid = true;
	tipoEntidad.capacity = ESTRUCTURA_DEFAULT_CAPACITY;
	tipoEntidad.behaviour = ESTRUCTURA_DEFAULT_BEHAVIOUR;
	tipoEntidad.health = ESTRUCTURA_DEFAULT_HEALTH;
	tipoEntidad.armour = ESTRUCTURA_DEFAULT_ARMOUR;
	tipoEntidad.products = std::vector<TagProduct>();
	tipoEntidad.resource_name = ESTRUCTURA_DEFAULT_RESOURCE_NAME;
	tipoEntidad.hit_force = ESTRUCTURA_DEFAULT_HIT_FORCE;
	tipoEntidad.hit_radius = ESTRUCTURA_DEFAULT_HIT_RADIUS;
}
