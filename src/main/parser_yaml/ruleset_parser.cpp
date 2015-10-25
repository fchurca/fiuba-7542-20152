#include "ruleset_parser.h"

RulesetParser::RulesetParser(std::string filename, std::string filenameDefault) : GenericParser(filename, filenameDefault) {
}

RulesetParser::~RulesetParser(void) {

}

std::vector<TagTipoEntidad> RulesetParser::getTiposEntidades() {
	Logger::getInstance()->writeInformation("YAML-CPP:Se obtiene informacion de los tipos de entidades.");
	std::vector<TagTipoEntidad> tiposDeEntidades;
	if (this->doc.FindValue("tipos_entidades")) {
		const YAML::Node& tipos = this->doc["tipos_entidades"];
		if (tipos.Type() == YAML::NodeType::Sequence) {
			for (unsigned int i = 0; i < tipos.size(); i++) {
				Logger::getInstance()->writeInformation("YAML-CPP:Se obtiene informacion del tipo de entidad numero." + intToString(i));
				TagTipoEntidad tipoEntidad;
				setTipoEntidad(tipos[i], tipoEntidad, i);
				tiposDeEntidades.push_back(tipoEntidad);
			}
		}
		else {
			Logger::getInstance()->writeWarning("YAML-CPP:El tag de tipos de entidad no es del tipo Sequence. Ubicar" + ubicarNodo(tipos.GetMark()));
			Logger::getInstance()->writeInformation("YAML-CPP:No se toman tipos de entidad");
		}
	}
	else {
		Logger::getInstance()->writeWarning("YAML-CPP:El tag de tipos no existe en el archivo.");
		Logger::getInstance()->writeInformation("YAML-CPP:No se toman tipos de entidad");
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

void RulesetParser::setTipoEntidad(const YAML::Node& node, TagTipoEntidad& tipoEntidad, int i) {
	if (node.Type() == YAML::NodeType::Map) {
		if (!obtenerValorScalarAlfaNumerico(node, "nombre", tipoEntidad.nombre)) {
			Logger::getInstance()->writeInformation("YAML-CPP:El nombre del tipo de entidad se toma por default.");
			tipoEntidad.nombre = ENTIDAD_DEFAULT_NOMBRE + intToString(i);
		}
		if ((!obtenerValorScalarAlfaNumerico(node, "imagen", tipoEntidad.imagen))
			|| (!obtenerValorScalarNumericoPositivo(node, "pixel_ref_x", tipoEntidad.pixel_ref_x))
			|| (!obtenerValorScalarNumericoPositivo(node, "pixel_ref_y", tipoEntidad.pixel_ref_y))
			|| (!obtenerValorScalarNumericoPositivo(node, "ancho_sprite", tipoEntidad.ancho_sprite))
			|| (!obtenerValorScalarNumericoPositivo(node, "alto_sprite", tipoEntidad.alto_sprite))
			|| (!obtenerValorScalarNumericoPositivo(node, "cantidad_sprites", tipoEntidad.cantidad_sprites))) {
			Logger::getInstance()->writeWarning("YAML-CPP:Datos de la imagen del tipo de entidad invalidos, se toman por default (path, pixel_ref_x, pixel_ref_y, ancho_sprite, alto_sprite, cantidad_sprites).");
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
			tipoEntidad.speed = VELOCIDAD_PERSONAJE_DEFAULT;
		}
		tipoEntidad.solid = true;
		tipoEntidad.capacity = ENTIDAD_DEFAULT_CAPACITY;
	}
	else {
		Logger::getInstance()->writeWarning("YAML-CPP:el contenido del tipo de entidad no es del tipo Map. Ubicar" + ubicarNodo(node.GetMark()));
		setTipoEntidadDefault(tipoEntidad, i);
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
		tipoTerreno.ancho_base = TERRENO_DEFAULT_ANCHO_BASE;
		tipoTerreno.alto_base = TERRENO_DEFAULT_ALTO_BASE;

		if (!obtenerValorScalarNumericoPositivo(node, "fps", tipoTerreno.fps)) {
			Logger::getInstance()->writeWarning("YAML-CPP: Se toma por default (fps).");
			tipoTerreno.fps = TERRENO_DEFAULT_FPS;
		}
		if (!obtenerValorScalarNumericoPositivo(node, "delay", tipoTerreno.delay)) {
			Logger::getInstance()->writeWarning("YAML-CPP: Se toma por default (delay).");
			tipoTerreno.delay = TERRENO_DEFAULT_DELAY;
		}
		tipoTerreno.sight_radius = TERRENO_DEFAULT_SIGHT_RADIUS;
		tipoTerreno.speed = TERRENO_DEFAULT_SPEED;
		string solid;
		if (!obtenerValorScalarAlfaNumerico(node, "solid", solid) || (solid != "true")) {
			Logger::getInstance()->writeWarning("YAML-CPP: Se toma por default valor false(solid).");
			tipoTerreno.solid = false;
		}
		else
			tipoTerreno.solid = true;
		tipoTerreno.capacity = TERRENO_DEFAULT_CAPACITY;
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
		tipoRecurso.cantidad_sprites = RECURSO_DEFAULT_CANTIDAD_SPRITES;
		if ((!obtenerValorScalarNumericoPositivo(node, "ancho_base", tipoRecurso.ancho_base))
			|| (!obtenerValorScalarNumericoPositivo(node, "alto_base", tipoRecurso.alto_base))) {
			Logger::getInstance()->writeWarning("YAML-CPP:Datos de la imagen del tipo de terreno invalidos, se toman por default (alto_base, ancho_base).");
			tipoRecurso.ancho_base = TERRENO_DEFAULT_ANCHO_BASE;
			tipoRecurso.alto_base = TERRENO_DEFAULT_ALTO_BASE;
		}
		tipoRecurso.fps = RECURSO_DEFAULT_FPS;
		tipoRecurso.delay = RECURSO_DEFAULT_DELAY;
		tipoRecurso.sight_radius = RECURSO_DEFAULT_SIGHT_RADIUS;
		tipoRecurso.speed = RECURSO_DEFAULT_SPEED;
		tipoRecurso.solid = false;
		if (!obtenerValorScalarNumericoPositivo(node, "capacidad", tipoRecurso.capacity)) {
			Logger::getInstance()->writeWarning("YAML-CPP: Se toma por default (capacity).");
			tipoRecurso.capacity = RECURSO_DEFAULT_CAPACITY;
		}
	}
	else {
		Logger::getInstance()->writeWarning("YAML-CPP:El contenido del tipo de terreno ad no es del tipo Map. Ubicar" + ubicarNodo(node.GetMark()));
		setTipoRecursoDefault(tipoRecurso, i);
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
	tipoRecurso.solid = false;
	tipoRecurso.capacity = RECURSO_DEFAULT_CAPACITY;
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
}


void RulesetParser::setTipoEntidadDefault(TagTipoEntidad& tipoEntidad, int i) {
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
	tipoEntidad.speed = VELOCIDAD_PERSONAJE_DEFAULT;
	tipoEntidad.solid = true;
	tipoEntidad.capacity = ENTIDAD_DEFAULT_CAPACITY;
}
