#include <cmath>

#include "parser_yaml.h"


ParserYAML::ParserYAML(std::string filename) {
	this->filename = filename;
}

void ParserYAML::parse() {
	Logger::getInstance()->writeInformation("YAML-CPP:Inicia el parseo del archivo de configuracion.");
	std::ifstream fin(this->filename);
	if(!fin) {
		Logger::getInstance()->writeError("YAML-CPP:El archivo de configuracion no existe o no pudo ser abierto.");
		Logger::getInstance()->writeInformation("YAML-CPP: se toma un archivo de escenario default.");
		setArchivoDefault();
	}
	else if (fin.peek() == EOF) {
		Logger::getInstance()->writeError("YAML-CPP:El archivo de configuracion se encuentra vacio.");
		Logger::getInstance()->writeInformation("YAML-CPP: se toma un archivo de escenario default.");
		setArchivoDefault();
	} 
	else {
		try{
			YAML::Parser parser(fin);
			parser.GetNextDocument(this->doc);
			if(this->doc.Type() != YAML::NodeType::Map) {
				Logger::getInstance()->writeError("YAML-CPP:La estructura del archivo de configuracion no es correcta.");
				Logger::getInstance()->writeInformation("YAML-CPP: se toma un archivo de escenario default.");
				setArchivoDefault();
			}
			if(fin.is_open())
				fin.close();
		}
		catch(YAML::ParserException &e) { 
			if(fin.is_open())
				fin.close();
			Logger::getInstance()->writeError("YAML-CPP:" + std::string(e.what()));
			Logger::getInstance()->writeInformation("YAML-CPP: se toma un archivo de escenario default.");
			setArchivoDefault();
		}
	}
}

TagConfiguracion2 ParserYAML::getConfiguracion() {
	Logger::getInstance()->writeInformation("YAML-CPP:Se obtiene la informacion de configuracion.");
	TagConfiguracion2 configuracion;
	if(this->doc.FindValue("configuracion")) {
		const YAML::Node& conf = this->doc["configuracion"];
		if(conf.Type() == YAML::NodeType::Map) {
			setConfiguracion(conf,configuracion);
		}
		else{
			Logger::getInstance()->writeWarning("YAML-CPP:El tag de configuracion no es del tipo Map. Ubicar" + ubicarNodo(conf.GetMark()));
			setConfiguracionDefault(configuracion);
		}
	}
	else{
		Logger::getInstance()->writeWarning("YAML-CPP:El tag de configuracion no existe en el archivo.");
		setConfiguracionDefault(configuracion);
	}
	return configuracion;
}

TagPantalla2 ParserYAML::getPantalla() {
	Logger::getInstance()->writeInformation("YAML-CPP:Se obtiene la informacion de la pantalla.");
	TagPantalla2 pantalla;
	if(this->doc.FindValue("pantalla")) {
		const YAML::Node& pant = this->doc["pantalla"];
		if(pant.Type() == YAML::NodeType::Map) {
			setPantalla(pant,pantalla);
		}
		else{
			Logger::getInstance()->writeWarning("YAML-CPP:El tag de pantalla no es del tipo Map. Ubicar" + ubicarNodo(pant.GetMark()));
			setPantallaDefault(pantalla);
		}
	}
	else{
		Logger::getInstance()->writeWarning("YAML-CPP:El tag de pantalla no existe en el archivo.");
		setPantallaDefault(pantalla);
	}
	return pantalla;
}

std::vector<TagTipoEntidad2> ParserYAML::getTiposEntidades() {
	Logger::getInstance()->writeInformation("YAML-CPP:Se obtiene informacion de los tipos de entidades.");
	std::vector<TagTipoEntidad2> tiposDeEntidades;
	if(this->doc.FindValue("tipos_entidades")) {
		const YAML::Node& tipos = this->doc["tipos_entidades"];
		if(tipos.Type() == YAML::NodeType::Sequence) {
			for(unsigned int i = 0; i < tipos.size(); i++) {
				Logger::getInstance()->writeInformation("YAML-CPP:Se obtiene informacion del tipo de entidad numero."  + intToString(i));
				TagTipoEntidad2 tipoEntidad;
				setTipoEntidad(tipos[i], tipoEntidad, i);
				tiposDeEntidades.push_back(tipoEntidad);
			}
		}
		else{
			Logger::getInstance()->writeWarning("YAML-CPP:El tag de tipos de entidad no es del tipo Sequence. Ubicar" + ubicarNodo(tipos.GetMark()));
			Logger::getInstance()->writeInformation("YAML-CPP:No se toman tipos de entidad");
		}
	}
	else{
		Logger::getInstance()->writeWarning("YAML-CPP:El tag de tipos no existe en el archivo.");
		Logger::getInstance()->writeInformation("YAML-CPP:No se toman tipos de entidad");
	}
	return tiposDeEntidades;
}

std::vector<TagTipoEntidad2> ParserYAML::getTiposTerrenos() {
	Logger::getInstance()->writeInformation("YAML-CPP:Se obtiene informacion de los tipos de terrenos.");
	std::vector<TagTipoEntidad2> tiposDeTerrenos;
	if(this->doc.FindValue("tipos_terrenos")) {
		const YAML::Node& tipos = this->doc["tipos_terrenos"];
		if(tipos.Type() == YAML::NodeType::Sequence) {
			for(unsigned int i = 0; i < tipos.size(); i++) {
				Logger::getInstance()->writeInformation("YAML-CPP:Se obtiene informacion del tipo de terreno numero."  + intToString(i));
				TagTipoEntidad2 tipoTerreno;
				setTipoTerreno(tipos[i], tipoTerreno, i);
				tiposDeTerrenos.push_back(tipoTerreno);
			}
		}
		else{
			Logger::getInstance()->writeWarning("YAML-CPP:El tag de tipos de terrenos no es del tipo Sequence. Ubicar" + ubicarNodo(tipos.GetMark()));
			Logger::getInstance()->writeInformation("YAML-CPP:No se toman tipos de terreno");
		}
	}
	else{
		Logger::getInstance()->writeWarning("YAML-CPP:El tag de tipos de terrenos no existe en el archivo.");
		Logger::getInstance()->writeInformation("YAML-CPP:No se toman tipos de terreno");
	}
	return tiposDeTerrenos;
}
std::vector<TagTipoEntidad2> ParserYAML::getTiposRecursos() {
	Logger::getInstance()->writeInformation("YAML-CPP:Se obtiene informacion de los tipos de recursos.");
	std::vector<TagTipoEntidad2> tiposDeRecursos;
	if (this->doc.FindValue("tipos_recursos")) {
		const YAML::Node& tipos = this->doc["tipos_recursos"];
		if (tipos.Type() == YAML::NodeType::Sequence) {
			for (unsigned int i = 0; i < tipos.size(); i++) {
				Logger::getInstance()->writeInformation("YAML-CPP:Se obtiene informacion del tipo de recurso numero." + intToString(i));
				TagTipoEntidad2 tipoRecurso;
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

TagEscenario2 ParserYAML::getEscenario() {
	Logger::getInstance()->writeInformation("YAML-CPP:Se obtiene informacion de escenario.");
	TagEscenario2 escenario;
	if(this->doc.FindValue("escenario")) { 
		const YAML::Node& esc = this->doc["escenario"];
		if(esc.Type() == YAML::NodeType::Map) {
			setEscenario(esc,escenario);
		}
		else{
			Logger::getInstance()->writeWarning("YAML-CPP:El tag de escenario no es del tipo Map. Ubicar" + ubicarNodo(esc.GetMark()));
			setEscenarioDefault(escenario);
		}
	}
	else{
		Logger::getInstance()->writeWarning("YAML-CPP:El archivo de configuracion no tiene una configuracion del escenario.");	
		setEscenarioDefault(escenario);
	}
	return escenario;
}

ParserYAML::~ParserYAML(void) {

}

void ParserYAML::setConfiguracion (const YAML::Node& node, TagConfiguracion2& configuracion) {
	if(node.Type() == YAML::NodeType::Map) {
		if(!obtenerValorScalarNumericoPositivo(node, "dt", configuracion.dt)){
			Logger::getInstance()->writeWarning("YAML-CPP: Se toma por default (dt).");
			configuracion.dt = DT_DEFAULT;
		}
	}
	else{
		Logger::getInstance()->writeWarning("YAML-CPP:El contenido del tag de configuracion no es del tipo Map. Ubicar" + ubicarNodo(node.GetMark()));
		setConfiguracionDefault(configuracion);
	}	
}

void ParserYAML::setConfiguracionDefault (TagConfiguracion2& configuracion) {
	Logger::getInstance()->writeWarning("YAML-CPP:Se toma configuracion por default.");
	configuracion.dt = DT_DEFAULT;
}


void ParserYAML::setPantalla (const YAML::Node& node, TagPantalla2& pantalla) {
	if(node.Type() == YAML::NodeType::Map) {
		if ((!obtenerValorScalarNumericoPositivo(node, "ancho", pantalla.ancho)) || (!obtenerValorScalarNumericoPositivo(node, "alto", pantalla.alto))) {
			Logger::getInstance()->writeWarning("YAML-CPP: Se toma por default (ancho y alto pantalla).");
			pantalla.alto = ALTO_DEFAULT;
			pantalla.ancho = ANCHO_DEFAULT;
		}
		if (!obtenerValorScalarNumericoPositivo(node, "margen_scroll", pantalla.margen_scroll)) {
			Logger::getInstance()->writeWarning("YAML-CPP: Se toma por default (margen scroll).");
			pantalla.margen_scroll = MARGEN_SCROLL_DEFAULT;
		}
		if (!obtenerValorScalarNumericoPositivo(node, "velocidad_scroll", pantalla.velocidad_scroll)) {
			Logger::getInstance()->writeWarning("YAML-CPP: Se toma por default (velocidad scroll).");
			pantalla.velocidad_scroll = VELOCIDAD_SCROLL_DEFAULT;
		}
	}
	else{
		Logger::getInstance()->writeWarning("YAML-CPP:El contenido del tag de pantalla no es del tipo Map. Ubicar" + ubicarNodo(node.GetMark()));
		setPantallaDefault(pantalla);
	}	
}

void ParserYAML::setPantallaDefault (TagPantalla2& pantalla) {
	Logger::getInstance()->writeInformation("YAML-CPP:Se toma pantalla por default.");
	pantalla.alto = ALTO_DEFAULT;
	pantalla.ancho = ANCHO_DEFAULT;
	pantalla.velocidad_scroll = VELOCIDAD_SCROLL_DEFAULT;
	pantalla.margen_scroll = MARGEN_SCROLL_DEFAULT;
}

void ParserYAML::setTipoEntidad (const YAML::Node& node, TagTipoEntidad2& tipoEntidad, int i) {
	if(node.Type() == YAML::NodeType::Map) {
		if(!obtenerValorScalarAlfaNumerico(node, "nombre", tipoEntidad.nombre)) {
			Logger::getInstance()->writeInformation("YAML-CPP:El nombre del tipo de entidad se toma por default.");
			tipoEntidad.nombre = ENTIDAD_DEFAULT_NOMBRE + intToString(i);
		}
		if((!obtenerValorScalarAlfaNumerico(node, "imagen", tipoEntidad.imagen))
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
		if(!obtenerValorScalarNumericoPositivo(node, "ancho_base", tipoEntidad.ancho_base)){
			Logger::getInstance()->writeWarning("YAML-CPP: Se toma por default (ancho_base).");
			tipoEntidad.ancho_base = ENTIDAD_DEFAULT_ANCHO_BASE;
		}
		if(!obtenerValorScalarNumericoPositivo(node, "alto_base", tipoEntidad.alto_base)){
			Logger::getInstance()->writeWarning("YAML-CPP: Se toma por default (alto_base).");
			tipoEntidad.alto_base = ENTIDAD_DEFAULT_ALTO_BASE;
		}

		if(!obtenerValorScalarNumericoPositivo(node, "fps", tipoEntidad.fps)){
			Logger::getInstance()->writeWarning("YAML-CPP: Se toma por default (fps).");
			tipoEntidad.fps = ENTIDAD_DEFAULT_FPS;
		}
		if(!obtenerValorScalarNumericoPositivo(node, "delay", tipoEntidad.delay)){
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
	else{
		Logger::getInstance()->writeWarning("YAML-CPP:el contenido del tipo de entidad no es del tipo Map. Ubicar" + ubicarNodo(node.GetMark()));
		setTipoEntidadDefault(tipoEntidad,i);
	}
}

void ParserYAML::setTipoTerreno (const YAML::Node& node, TagTipoEntidad2& tipoTerreno, int i) {
	if(node.Type() == YAML::NodeType::Map) {
		if(!obtenerValorScalarAlfaNumerico(node, "nombre", tipoTerreno.nombre)) {
			Logger::getInstance()->writeInformation("YAML-CPP: el nombre del tipo de entidad se toma por default.");
			tipoTerreno.nombre = TERRENO_DEFAULT_NOMBRE + intToString(i);
		}
		if((!obtenerValorScalarAlfaNumerico(node, "imagen", tipoTerreno.imagen))
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

		if(!obtenerValorScalarNumericoPositivo(node, "fps", tipoTerreno.fps)){
			Logger::getInstance()->writeWarning("YAML-CPP: Se toma por default (fps).");
			tipoTerreno.fps = TERRENO_DEFAULT_FPS;
		}
		if(!obtenerValorScalarNumericoPositivo(node, "delay", tipoTerreno.delay)){
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
	else{
		Logger::getInstance()->writeWarning("YAML-CPP:El contenido del tipo de terreno ad no es del tipo Map. Ubicar" + ubicarNodo(node.GetMark()));
		setTipoTerrenoDefault(tipoTerreno,i);
	}
}

void ParserYAML::setTipoRecurso(const YAML::Node& node, TagTipoEntidad2& tipoRecurso, int i) {
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
			|| (!obtenerValorScalarNumericoPositivo(node, "alto_base", tipoRecurso.alto_base))){
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

void ParserYAML::setTipoRecursoDefault(TagTipoEntidad2& tipoRecurso, int i) {
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

void ParserYAML::setTipoTerrenoDefault(TagTipoEntidad2& tipoEntidad, int i) {
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


void ParserYAML::setTipoEntidadDefault (TagTipoEntidad2& tipoEntidad, int i) {
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

void ParserYAML::setEntidad(const YAML::Node& node, TagEntidad2& entidad) {
	if(node.Type() == YAML::NodeType::Map) {
		if(!obtenerValorScalarNumericoPositivo(node, "x", entidad.pos_x)) {
			Logger::getInstance()->writeWarning("YAML-CPP:La posicion x de la entidad se toma por defecto.");
			entidad.pos_x = ENTIDAD_DEFAULT_POSX;
		}

		if(!obtenerValorScalarNumericoPositivo(node, "y", entidad.pos_y)) {
			Logger::getInstance()->writeWarning("YAML-CPP:La posicion y de la entidad se toma por defecto.");
			entidad.pos_y = ENTIDAD_DEFAULT_POSY;
		}

		if ((!obtenerValorScalarAlfaNumerico(node, "tipo", entidad.tipoEntidad))|| (entidad.tipoEntidad.empty())){
			Logger::getInstance()->writeWarning("YAML-CPP:El tipo de la entidad se toma por defecto.");
			entidad.tipoEntidad = ENTIDAD_DEFAULT_NOMBRE;
		}
	}
	else{
		Logger::getInstance()->writeWarning("YAML-CPP:El contenido de la entidad no es del tipo Map. Ubicar" + ubicarNodo(node.GetMark()));
		Logger::getInstance()->writeInformation("YAML-CPP:Se toman valores por default para esa entidad");
		entidad.tipoEntidad = ENTIDAD_DEFAULT_NOMBRE;
		entidad.pos_y = ENTIDAD_DEFAULT_POSY;
		entidad.pos_x = ENTIDAD_DEFAULT_POSX;
	}
}

void ParserYAML::setEscenario(const YAML::Node& node, TagEscenario2& escenario) {
	if(node.Type() == YAML::NodeType::Map) {
		if(!obtenerValorScalarAlfaNumerico(node, "nombre", escenario.nombre)) {
			Logger::getInstance()->writeWarning("YAML-CPP:El nombre del escenario se toma por default.");
			escenario.nombre = ESCENARIO_DEFAULT_NOMBRE;
		}

		if((!obtenerValorScalarNumericoPositivo(node, "size_x", escenario.size_x))||(!obtenerValorScalarNumericoPositivo(node, "size_y", escenario.size_y))) {
			Logger::getInstance()->writeInformation("YAML-CPP:Se toman por default los datos del tamanio del escenario.");
			escenario.size_x = ESCENARIO_DEFAULT_SIZE_X;
			escenario.size_y = ESCENARIO_DEFAULT_SIZE_Y;
		}
		if (!obtenerValorScalarNumericoPositivo(node, "max_resources", escenario.max_resources)) {
			Logger::getInstance()->writeWarning("YAML-CPP:Max_resources del escenario se toma por default.");
			escenario.max_resources = ESCENARIO_DEFAULT_MAXRESOURCES;
		}

		std::vector<TagEntidad2> entidades;
		if(node.FindValue("entidades")) { 
			const YAML::Node& ent = node["entidades"];
			if(ent.Type() == YAML::NodeType::Sequence) {
				for(unsigned int i = 0; i < ent.size(); i++) {
					TagEntidad2 entidad;
					setEntidad(ent[i],entidad);
					entidades.push_back(entidad);
				}
			}
			else{
				Logger::getInstance()->writeWarning("YAML-CPP:El tag entidades no es del tipo secuencia.");
				Logger::getInstance()->writeInformation("YAML-CPP: No se toman entidades.");
			}
		}
		else{
			Logger::getInstance()->writeWarning("YAML-CPP:El tag entidades no existe en el archivo.");
			Logger::getInstance()->writeInformation("YAML-CPP: No se toman entidades.");
		}
		escenario.entidades = entidades;

		std::vector<TagEntidad2> terrenos;
		if(node.FindValue("terrenos")) { 
			const YAML::Node& ter = node["terrenos"];
			if(ter.Type() == YAML::NodeType::Sequence) {
				for(unsigned int i = 0; i < ter.size(); i++) {
					TagEntidad2 terreno;
					setEntidad(ter[i],terreno);
					terrenos.push_back(terreno);
				}
			}
			else{
				Logger::getInstance()->writeWarning("YAML-CPP:El tag terrenos no es del tipo secuencia.");
				Logger::getInstance()->writeInformation("YAML-CPP: No se toman terrenos.");
			}
		}
		else{
			Logger::getInstance()->writeWarning("YAML-CPP:El tag terrenos no existe en el archivo.");
			Logger::getInstance()->writeInformation("YAML-CPP: No se toman terrenos.");
		}
		escenario.terrenos = terrenos;

		//TagEntidad2 protagonista;
		//if(node.FindValue("protagonista")) { 
		//	const YAML::Node& pro = node["protagonista"];
		//	if(pro.Type() == YAML::NodeType::Sequence) {
		//		if(pro.size() == 1) {
		//			setEntidad(pro[0], protagonista);
		//			escenario.protagonista = protagonista;
		//		}
		//		else{
		//			setProtagonistaDefault(protagonista);
		//			Logger::getInstance()->writeWarning("YAML-CPP: El tag protagonista posee mas de un elemento. Ubicar:" + ubicarNodo(pro.GetMark()));
		//		}
		//	}
		//	else{
		//		setProtagonistaDefault(protagonista);
		//		Logger::getInstance()->writeWarning("YAML-CPP: El tag de protagonista del escenario no es del tipo Sequence. Ubicar:" + ubicarNodo(pro.GetMark()));
		//	}
		//}
		//else{
		//	setProtagonistaDefault(protagonista);
		//	Logger::getInstance()->writeWarning("YAML-CPP: El tag de protagonista del escenario no existe.");
		//}
		//escenario.protagonista=protagonista;
		std::vector<TagJugador2> jugadores;
		if (node.FindValue("jugadores")) {
			const YAML::Node& jugs = node["jugadores"];
			if (jugs.Type() == YAML::NodeType::Sequence) {
				for (unsigned int i = 0; i < jugs.size(); i++) {
					TagJugador2 jugador;
					setJugador(jugs[i], jugador, i);
					jugadores.push_back(jugador);
				}
			}
			else {
				Logger::getInstance()->writeWarning("YAML-CPP:El tag jugadores no es del tipo secuencia.");
				Logger::getInstance()->writeInformation("YAML-CPP: No se toman jugadores.");
			}
		}
		else {
			Logger::getInstance()->writeWarning("YAML-CPP:El tag jugadores no existe en el archivo.");
			Logger::getInstance()->writeInformation("YAML-CPP: No se toman jugadores.");
		}
		escenario.jugadores = jugadores;
	}
	else{
		Logger::getInstance()->writeWarning("YAML-CPP: El tag de escenario no es del tipo Map. Ubicar:" + ubicarNodo(node.GetMark()));
		setEscenarioDefault(escenario);
	}
}

void ParserYAML::setJugador(const YAML::Node& node, TagJugador2& jugador, int i) {
	if (node.Type() == YAML::NodeType::Map) {
		if (!obtenerValorScalarAlfaNumerico(node, "name", jugador.name)) {
			Logger::getInstance()->writeWarning("YAML-CPP:El nombre del jugador se toma por defecto.");
			jugador.name = DEFAULT_PLAYER_NAME + intToString(i);
		}

		std::string isHuman;
		if (!obtenerValorScalarAlfaNumerico(node, "isHuman", isHuman) || (isHuman != "true")){
			Logger::getInstance()->writeWarning("YAML-CPP:El isHuman del jugador se toma por defecto false.");
			jugador.isHuman = false;
		}
		else
			jugador.isHuman = true;

		std::vector<TagEntidad2> entidades;
		if (node.FindValue("entidades")) {
			const YAML::Node& ent = node["entidades"];
			if (ent.Type() == YAML::NodeType::Sequence) {
				for (unsigned int i = 0; i < ent.size(); i++) {
					TagEntidad2 entidad;
					setEntidad(ent[i], entidad);
					entidades.push_back(entidad);
				}
			}
			else {
				Logger::getInstance()->writeWarning("YAML-CPP:El tag entidades no es del tipo secuencia.");
				Logger::getInstance()->writeInformation("YAML-CPP: No se toman entidades.");
			}
		}
		else {
			Logger::getInstance()->writeWarning("YAML-CPP:El tag entidades no existe en el archivo.");
			Logger::getInstance()->writeInformation("YAML-CPP: No se toman entidades.");
		}
		jugador.entidades = entidades;
	}
	else {
		Logger::getInstance()->writeWarning("YAML-CPP:El contenido del jugador no es del tipo Map. Ubicar" + ubicarNodo(node.GetMark()));
		Logger::getInstance()->writeInformation("YAML-CPP:Se toman valores por default para ese jugador");
		jugador.name = DEFAULT_PLAYER_NAME + intToString(i);
		jugador.isHuman = false;
		std::vector<TagEntidad2> entitiesDefault;
		jugador.entidades = entitiesDefault;
	}
}

void ParserYAML::setProtagonistaDefault (TagEntidad2& protagonista) {
	Logger::getInstance()->writeWarning("YAML-CPP: Se toma protagonista por default.");
	protagonista.tipoEntidad = PROTAGONISTA_DEFAULT_NOMBRE;
	protagonista.pos_x = PROTAGONISTA_DEFAULT_POSX;
	protagonista.pos_y = PROTAGONISTA_DEFAULT_POSY;
}

void ParserYAML::setEscenarioDefault (TagEscenario2& escenario) {
	Logger::getInstance()->writeWarning("YAML-CPP: Se toma escenario por default.");
	std::vector<TagEntidad2> entidades;
	std::vector<TagEntidad2> terrenos;
	std::vector<TagJugador2> jugadores;

	//TagEntidad2 protagonista;
	//escenario.protagonista = protagonista;
	//setProtagonistaDefault(protagonista);

	escenario.nombre = ESCENARIO_DEFAULT_NOMBRE;
	escenario.size_x = ESCENARIO_DEFAULT_SIZE_X;
	escenario.size_y= ESCENARIO_DEFAULT_SIZE_Y;
	escenario.max_resources = ESCENARIO_DEFAULT_MAXRESOURCES;
	escenario.entidades = entidades;
	escenario.terrenos = terrenos;
	escenario.jugadores = jugadores;
}

void ParserYAML::setArchivoDefault() {
	// Este metodo no puede fallar, el archivo default no puede contener errores.
	this->filename = CONFIG_FILE_PATH_DEFAULT;
	std::ifstream fin(this->filename.c_str());
	YAML::Parser parser(fin);
	parser.GetNextDocument(this->doc);
	if(fin.is_open())
		fin.close();
}


bool ParserYAML::esNumero(std::string numero) { 
	int i = 0;
	int cantDec = 0;
	int largo = numero.length();
	if(numero.empty() || ((numero[0] != '-') && (!isdigit(numero[0])))) {
		return false;
	}
	if(numero[0] == '-') {
		if(largo < 2)
			return false;
		if(!isdigit(numero[1]))
			return false;
		i = 1;
	}
	for(; i < largo; i++){
		if(!isdigit(numero[i]) && numero[i] != '.') 
			return false; 
		else 
			if(numero[i] == '.')
				cantDec++;
	}
	if(cantDec > 1)
		return false;
	return true; 
}

bool ParserYAML::obtenerValorScalarNumericoPositivo(const YAML::Node & node, std::string tag, unsigned int & salida) {
	std::string numero;
	double num;
	if(node.FindValue(tag)) {
		const YAML::Node& nodo_tag = node[tag];
		if (nodo_tag.Type() == YAML::NodeType::Scalar) {
			nodo_tag >> numero;
			if (esNumero(numero)) {
				nodo_tag >> num;
				if(num >= 0) {
					double p_entera; 
					double p_decimal; 
					p_decimal = modf(num, &p_entera);
					if(p_decimal == 0){
						salida = p_entera;
						return true;
					}
					else
						Logger::getInstance()->writeWarning("YAML-CPP:El contenido ubicado en: " + tag + " es un numero decimal. Ubicar" + ubicarNodo(nodo_tag.GetMark()));
				}
				else
					Logger::getInstance()->writeWarning("YAML-CPP:El contenido ubicado en: " + tag + " es un numero negativo. Ubicar" + ubicarNodo(nodo_tag.GetMark()));
			}
			else
				Logger::getInstance()->writeWarning("YAML-CPP:El contenido ubicado en: " + tag + " no es un numero. Ubicar" + ubicarNodo(nodo_tag.GetMark()));
		}
		else
			Logger::getInstance()->writeWarning("YAML-CPP:El contenido ubicado en: " + tag + " no es del tipo Scalar. Ubicar" + ubicarNodo(nodo_tag.GetMark()));
	}
	else
		Logger::getInstance()->writeWarning("YAML-CPP:El: " + tag + " no existe en el nodo. Ubicar" + ubicarNodo(node.GetMark()));
	return false;
}
bool ParserYAML::obtenerValorScalarNumericoPositivo(const YAML::Node & node, std::string tag, long & salida) {
	std::string numero;
	double num;
	if (node.FindValue(tag)) {
		const YAML::Node& nodo_tag = node[tag];
		if (nodo_tag.Type() == YAML::NodeType::Scalar) {
			nodo_tag >> numero;
			if (esNumero(numero)) {
				nodo_tag >> num;
				if (num >= 0) {
					double p_entera;
					double p_decimal;
					p_decimal = modf(num, &p_entera);
					if (p_decimal == 0) {
						salida = p_entera;
						return true;
					}
					else
						Logger::getInstance()->writeWarning("YAML-CPP:El contenido ubicado en: " + tag + " es un numero decimal. Ubicar" + ubicarNodo(nodo_tag.GetMark()));
				}
				else
					Logger::getInstance()->writeWarning("YAML-CPP:El contenido ubicado en: " + tag + " es un numero negativo. Ubicar" + ubicarNodo(nodo_tag.GetMark()));
			}
			else
				Logger::getInstance()->writeWarning("YAML-CPP:El contenido ubicado en: " + tag + " no es un numero. Ubicar" + ubicarNodo(nodo_tag.GetMark()));
		}
		else
			Logger::getInstance()->writeWarning("YAML-CPP:El contenido ubicado en: " + tag + " no es del tipo Scalar. Ubicar" + ubicarNodo(nodo_tag.GetMark()));
	}
	else
		Logger::getInstance()->writeWarning("YAML-CPP:El: " + tag + " no existe en el nodo. Ubicar" + ubicarNodo(node.GetMark()));
	return false;
}

bool ParserYAML::obtenerValorScalarNumericoPositivo(const YAML::Node & node, std::string tag, double & salida) {
	std::string numero;
	double num;
	if(node.FindValue(tag)) {
		const YAML::Node& nodo_tag = node[tag];
		if (nodo_tag.Type() == YAML::NodeType::Scalar) {
			nodo_tag >> numero;
			if (esNumero(numero)) {
				nodo_tag >> num;
				if(num >= 0) {
					nodo_tag >> salida;
					return true;
				}
				else
					Logger::getInstance()->writeWarning("YAML-CPP:El contenido ubicado en: " + tag + " es un numero negativo. Ubicar" + ubicarNodo(nodo_tag.GetMark()));
			}
			else
				Logger::getInstance()->writeWarning("YAML-CPP:El contenido ubicado en: " + tag + " no es un numero. Ubicar" + ubicarNodo(nodo_tag.GetMark()));
		}
		else
			Logger::getInstance()->writeWarning("YAML-CPP:El contenido ubicado en: " + tag + " no es del tipo Scalar. Ubicar" + ubicarNodo(nodo_tag.GetMark()));
	}
	else
		Logger::getInstance()->writeWarning("YAML-CPP:El : " + tag + " no existe en el nodo. Ubicar" + ubicarNodo(node.GetMark()));
	return false;
}

bool ParserYAML::obtenerValorScalarAlfaNumerico(const YAML::Node & node, std::string tag, std::string & salida) {
	if(node.FindValue(tag)) {
		const YAML::Node& nodo_tag = node[tag];
		if (nodo_tag.Type() == YAML::NodeType::Scalar) {
			nodo_tag >> salida;
			return true;
		}
		else
			Logger::getInstance()->writeWarning("YAML-CPP:El contenido ubicado en: " + tag + " no es del tipo Scalar. Ubicar" + ubicarNodo(nodo_tag.GetMark()));
	}
	else
		Logger::getInstance()->writeWarning("YAML-CPP:El: " + tag + " no existe en el nodo. Ubicar" + ubicarNodo(node.GetMark()));
	return false;
}



std::string ParserYAML::ubicarNodo(const YAML::Mark marca_nodo) {
	std::string salida;
    salida = " Linea: " + intToString(marca_nodo.line+1) + " Columna: " + intToString(marca_nodo.column+1);
	return salida;
}

std::string ParserYAML::intToString(int i) {
	string resultado;          
	ostringstream aux;   
	aux << i;      
	resultado = aux.str();
	return resultado;
}


