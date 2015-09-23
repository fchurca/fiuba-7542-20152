#include <cmath>

#include "parser_yaml.h"


ParserYAML::ParserYAML(std::string filename) {
	this->filename = filename;
}

void ParserYAML::parse() {
	Logger::getInstance()->writeInformation("YAML-CPP:Inicia el parseo del archivo de configuracion.");
	std::ifstream fin(this->filename.c_str());
	if(!fin) {
		Logger::getInstance()->writeError("YAML-CPP:No existe el archivo de configuracion o no pudo ser abierto.");
		Logger::getInstance()->writeInformation("YAML-CPP: se toma un archivo de escenario default.");
		setArchivoDefault();
	}
	else if (fin.peek() == EOF) {
		Logger::getInstance()->writeError("YAML-CPP:El archivo de configuracion esta vacio.");
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
			std::string mensaje = std::string(e.what());
			if(fin.is_open())
				fin.close();
			Logger::getInstance()->writeError("YAML-CPP:" + mensaje);
			Logger::getInstance()->writeInformation("YAML-CPP: se toma un archivo de escenario default.");
			setArchivoDefault();
		}
	}
}

TagConfiguracion ParserYAML::getConfiguracion() {
	Logger::getInstance()->writeInformation("YAML-CPP:Se obtiene la informacion de configuracion.");
	TagConfiguracion configuracion;
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

TagPantalla ParserYAML::getPantalla() {
	Logger::getInstance()->writeInformation("YAML-CPP:Se obtiene la informacion de la pantalla.");
	TagPantalla pantalla;
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

std::vector<TagTipoEntidad> ParserYAML::getTiposEntidades() {
	Logger::getInstance()->writeInformation("YAML-CPP:Se obtiene informacion de los tipos de entidades.");
	std::vector<TagTipoEntidad> tiposDeEntidades;
	if(this->doc.FindValue("tipos_entidades")) {
		const YAML::Node& tipos = this->doc["tipos_entidades"];
		if(tipos.Type() == YAML::NodeType::Sequence) {
			for(unsigned int i = 0; i < tipos.size(); i++) {
				Logger::getInstance()->writeInformation("YAML-CPP:Se obtiene informacion del tipo de entidad numero."  + intToString(i));
				TagTipoEntidad tipoEntidad;
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

std::vector<TagTipoEntidad> ParserYAML::getTiposTerrenos() {
	Logger::getInstance()->writeInformation("YAML-CPP:Se obtiene informacion de los tipos de terrenos.");
	std::vector<TagTipoEntidad> tiposDeTerrenos;
	if(this->doc.FindValue("tipos_terrenos")) {
		const YAML::Node& tipos = this->doc["tipos_terrenos"];
		if(tipos.Type() == YAML::NodeType::Sequence) {
			for(unsigned int i = 0; i < tipos.size(); i++) {
				Logger::getInstance()->writeInformation("YAML-CPP:Se obtiene informacion del tipo de terreno numero."  + intToString(i));
				TagTipoEntidad tipoTerreno;
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

TagEscenario ParserYAML::getEscenario() {
	Logger::getInstance()->writeInformation("YAML-CPP:Se obtiene informacion de escenario.");
	TagEscenario escenario;
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

void ParserYAML::setConfiguracion (const YAML::Node& node, TagConfiguracion& configuracion) {
	if(node.Type() == YAML::NodeType::Map) {
		if(!validarScalarNumericoPositivo(node, "vel_personaje", configuracion.vel_personaje)){
			Logger::getInstance()->writeWarning("YAML-CPP: Se toma por default (velocidad personaje).");
			configuracion.vel_personaje = VELOCIDAD_PERSONAJE_DEFAULT;
		}
		if(!validarScalarNumericoPositivo(node, "margen_scroll", configuracion.margen_scroll)){
			Logger::getInstance()->writeWarning("YAML-CPP: Se toma por default (margen scroll).");
			configuracion.margen_scroll = MARGEN_SCROLL_DEFAULT;
		}
		if(!validarScalarNumericoPositivo(node, "velocidad_scroll", configuracion.velocidad_scroll)){
			Logger::getInstance()->writeWarning("YAML-CPP: Se toma por default (velocidad scroll).");
			configuracion.velocidad_scroll = VELOCIDAD_SCROLL_DEFAULT;
		}
		if(!validarScalarNumericoPositivo(node, "dt", configuracion.dt)){
			Logger::getInstance()->writeWarning("YAML-CPP: Se toma por default (dt).");
			configuracion.dt = DT_DEFAULT;
		}
	}
	else{
		Logger::getInstance()->writeWarning("YAML-CPP:El contenido del tag de configuracion no es del tipo Map. Ubicar" + ubicarNodo(node.GetMark()));
		setConfiguracionDefault(configuracion);
	}	
}

void ParserYAML::setConfiguracionDefault (TagConfiguracion& configuracion) {
	Logger::getInstance()->writeWarning("YAML-CPP:Se toma configuracion por default.");
	configuracion.margen_scroll = MARGEN_SCROLL_DEFAULT;
	configuracion.vel_personaje = VELOCIDAD_PERSONAJE_DEFAULT;
	configuracion.velocidad_scroll = VELOCIDAD_SCROLL_DEFAULT;
	configuracion.dt = DT_DEFAULT;
}


void ParserYAML::setPantalla (const YAML::Node& node, TagPantalla& pantalla) {
	if(node.Type() == YAML::NodeType::Map) {
		if((!validarScalarNumericoPositivo(node, "ancho", pantalla.ancho)) || (!validarScalarNumericoPositivo(node, "alto", pantalla.alto)))
			setPantallaDefault(pantalla);	
	}
	else{
		Logger::getInstance()->writeWarning("YAML-CPP:El contenido del tag de pantalla no es del tipo Map. Ubicar" + ubicarNodo(node.GetMark()));
		setPantallaDefault(pantalla);
	}	
}

void ParserYAML::setPantallaDefault (TagPantalla& pantalla) {
	Logger::getInstance()->writeInformation("YAML-CPP:Se toma pantalla por default.");
	pantalla.alto = ALTO_DEFAULT;
	pantalla.ancho = ANCHO_DEFAULT;
}

void ParserYAML::setTipoEntidad (const YAML::Node& node, TagTipoEntidad& tipoEntidad, int i) {
	if(node.Type() == YAML::NodeType::Map) {
		if(!validarScalarAlfaNumerico(node, "nombre", tipoEntidad.nombre)) {
			Logger::getInstance()->writeInformation("YAML-CPP:El nombre del tipo de entidad se toma por default.");
			tipoEntidad.nombre = ENTIDAD_DEFAULT_NOMBRE + intToString(i);
		}
		if((!validarScalarAlfaNumerico(node, "imagen", tipoEntidad.imagen))
			|| (!validarScalarNumericoPositivo(node, "pixel_ref_x", tipoEntidad.pixel_ref_x))
			|| (!validarScalarNumericoPositivo(node, "pixel_ref_y", tipoEntidad.pixel_ref_y))
			|| (!validarScalarNumericoPositivo(node, "ancho_sprite", tipoEntidad.ancho_sprite))
			|| (!validarScalarNumericoPositivo(node, "alto_sprite", tipoEntidad.alto_sprite))
			|| (!validarScalarNumericoPositivo(node, "cantidad_sprites", tipoEntidad.cantidad_sprites))) {
				Logger::getInstance()->writeWarning("YAML-CPP:Datos de la imagen del tipo de entidad invalidos, se toman por default (path, pixel_ref_x, pixel_ref_y, ancho_sprite, alto_sprite, cantidad_sprites).");
				tipoEntidad.imagen = ENTIDAD_DEFAULT_IMAGEN;
				tipoEntidad.pixel_ref_x = ENTIDAD_DEFAULT_PIXEL_REF_X;
				tipoEntidad.pixel_ref_y = ENTIDAD_DEFAULT_PIXEL_REF_Y;
				tipoEntidad.alto_sprite = ENTIDAD_DEFAULT_ALTO_SPRITE;
				tipoEntidad.ancho_sprite = ENTIDAD_DEFAULT_ANCHO_SPRITE;
				tipoEntidad.cantidad_sprites = ENTIDAD_DEFAULT_CANTIDAD_SPRITES;
		}
		if(!validarScalarNumericoPositivo(node, "ancho_base", tipoEntidad.ancho_base)){
			Logger::getInstance()->writeWarning("YAML-CPP: Se toma por default (ancho_base).");
			tipoEntidad.ancho_base = ENTIDAD_DEFAULT_ANCHO_BASE;
		}
		if(!validarScalarNumericoPositivo(node, "alto_base", tipoEntidad.alto_base)){
			Logger::getInstance()->writeWarning("YAML-CPP: Se toma por default (alto_base).");
			tipoEntidad.alto_base = ENTIDAD_DEFAULT_ALTO_BASE;
		}

		if(!validarScalarNumericoPositivo(node, "fps", tipoEntidad.fps)){
			Logger::getInstance()->writeWarning("YAML-CPP: Se toma por default (fps).");
			tipoEntidad.fps = ENTIDAD_DEFAULT_FPS;
		}
		if(!validarScalarNumericoPositivo(node, "delay", tipoEntidad.delay)){
			Logger::getInstance()->writeWarning("YAML-CPP: Se toma por default (delay).");
			tipoEntidad.delay = ENTIDAD_DEFAULT_DELAY;
		}
	}
	else{
		Logger::getInstance()->writeWarning("YAML-CPP:el contenido del tipo de entidad no es del tipo Map. Ubicar" + ubicarNodo(node.GetMark()));
		setTipoEntidadDefault(tipoEntidad);
	}
}

void ParserYAML::setTipoTerreno (const YAML::Node& node, TagTipoEntidad& tipoTerreno, int i) {
	if(node.Type() == YAML::NodeType::Map) {
		if(!validarScalarAlfaNumerico(node, "nombre", tipoTerreno.nombre)) {
			Logger::getInstance()->writeInformation("YAML-CPP: el nombre del tipo de entidad se toma por default.");
			tipoTerreno.nombre = TERRENO_DEFAULT_NOMBRE + intToString(i);
		}
		if((!validarScalarAlfaNumerico(node, "imagen", tipoTerreno.imagen))
			|| (!validarScalarNumericoPositivo(node, "pixel_ref_x", tipoTerreno.pixel_ref_x))
			|| (!validarScalarNumericoPositivo(node, "pixel_ref_y", tipoTerreno.pixel_ref_y))
			|| (!validarScalarNumericoPositivo(node, "ancho_sprite", tipoTerreno.ancho_sprite))
			|| (!validarScalarNumericoPositivo(node, "alto_sprite", tipoTerreno.alto_sprite))
			|| (!validarScalarNumericoPositivo(node, "cantidad_sprites", tipoTerreno.cantidad_sprites))) {
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

		if(!validarScalarNumericoPositivo(node, "fps", tipoTerreno.fps)){
			Logger::getInstance()->writeWarning("YAML-CPP: Se toma por default (fps).");
			tipoTerreno.fps = TERRENO_DEFAULT_FPS;
		}
		if(!validarScalarNumericoPositivo(node, "delay", tipoTerreno.delay)){
			Logger::getInstance()->writeWarning("YAML-CPP: Se toma por default (delay).");
			tipoTerreno.delay = TERRENO_DEFAULT_DELAY;
		}
	}
	else{
		Logger::getInstance()->writeWarning("YAML-CPP:El contenido del tipo de terreno ad no es del tipo Map. Ubicar" + ubicarNodo(node.GetMark()));
		setTipoEntidadDefault(tipoTerreno);
	}
}


void ParserYAML::setTipoEntidadDefault (TagTipoEntidad& tipoEntidad) {
	Logger::getInstance()->writeInformation("YAML-CPP:Se toma tipo de entidad por default.");
	tipoEntidad.nombre = ENTIDAD_DEFAULT_NOMBRE;
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
}

void ParserYAML::setEntidad(const YAML::Node& node, TagEntidad& entidad) {
	if(node.Type() == YAML::NodeType::Map) {
		if(!validarScalarNumericoPositivo(node, "x", entidad.pos_x)) {
			Logger::getInstance()->writeWarning("YAML-CPP:La posicion x de la entidad se toma por defecto.");
			entidad.pos_x = ENTIDAD_DEFAULT_POSX;
		}

		if(!validarScalarNumericoPositivo(node, "y", entidad.pos_y)) {
			Logger::getInstance()->writeWarning("YAML-CPP:La posicion y de la entidad se toma por defecto.");
			entidad.pos_y = ENTIDAD_DEFAULT_POSY;
		}

		if ((!validarScalarAlfaNumerico(node, "tipo", entidad.tipoEntidad))|| (entidad.tipoEntidad.empty())){
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

void ParserYAML::setEscenario(const YAML::Node& node, TagEscenario& escenario) {
	if(node.Type() == YAML::NodeType::Map) {
		if(!validarScalarAlfaNumerico(node, "nombre", escenario.nombre)) {
			Logger::getInstance()->writeWarning("YAML-CPP:El nombre del escenario se toma por default.");
			escenario.nombre = ESCENARIO_DEFAULT_NOMBRE;
		}

		if((!validarScalarNumericoPositivo(node, "size_x", escenario.size_x))||(!validarScalarNumericoPositivo(node, "size_y", escenario.size_y))) {
			Logger::getInstance()->writeInformation("YAML-CPP:Se toman por default los datos del tamanio del escenario.");
			escenario.size_x = ESCENARIO_DEFAULT_SIZE_X;
			escenario.size_y = ESCENARIO_DEFAULT_SIZE_Y;
		}

		std::vector<TagEntidad> entidades;
		if(node.FindValue("entidades")) { 
			const YAML::Node& ent = node["entidades"];
			if(ent.Type() == YAML::NodeType::Sequence) {
				for(unsigned int i = 0; i < ent.size(); i++) {
					TagEntidad entidad;
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

		std::vector<TagEntidad> terrenos;
		if(node.FindValue("terrenos")) { 
			const YAML::Node& ter = node["terrenos"];
			if(ter.Type() == YAML::NodeType::Sequence) {
				for(unsigned int i = 0; i < ter.size(); i++) {
					TagEntidad terreno;
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

		TagEntidad protagonista;
		if(node.FindValue("protagonista")) { 
			const YAML::Node& pro = node["protagonista"];
			if(pro.Type() == YAML::NodeType::Sequence) {
				if(pro.size() == 1) {
					setEntidad(pro[0], protagonista);
					escenario.protagonista = protagonista;
				}
				else{
					setProtagonistaDefault(protagonista);
					Logger::getInstance()->writeWarning("YAML-CPP: El tag protagonista posee mas de un elemento. Ubicar:" + ubicarNodo(pro.GetMark()));
				}
			}
			else{
				setProtagonistaDefault(protagonista);
				Logger::getInstance()->writeWarning("YAML-CPP: El tag de protagonista del escenario no es del tipo Sequence. Ubicar:" + ubicarNodo(pro.GetMark()));
			}
		}
		else{
			setProtagonistaDefault(protagonista);
			Logger::getInstance()->writeWarning("YAML-CPP: El tag de protagonista del escenario no existe.");
		}
		escenario.protagonista=protagonista;
	}
	else{
		Logger::getInstance()->writeWarning("YAML-CPP: El tag de escenario no es del tipo Map. Ubicar:" + ubicarNodo(node.GetMark()));
		setEscenarioDefault(escenario);
	}
}

void ParserYAML::setProtagonistaDefault (TagEntidad& protagonista) {
	Logger::getInstance()->writeWarning("YAML-CPP: Se toma protagonista por default.");
	protagonista.tipoEntidad = PROTAGONISTA_DEFAULT_NOMBRE;
	protagonista.pos_x = PROTAGONISTA_DEFAULT_POSX;
	protagonista.pos_y = PROTAGONISTA_DEFAULT_POSY;
}

void ParserYAML::setEscenarioDefault (TagEscenario& escenario) {
	Logger::getInstance()->writeWarning("YAML-CPP: Se toma escenario por default.");
	std::vector<TagEntidad> entidades;
	std::vector<TagEntidad> terrenos;
	TagEntidad protagonista;
	setProtagonistaDefault(protagonista);
	escenario.nombre = ESCENARIO_DEFAULT_NOMBRE;
	escenario.size_x = ESCENARIO_DEFAULT_SIZE_X;
	escenario.size_y= ESCENARIO_DEFAULT_SIZE_Y;
	escenario.entidades = entidades;
	escenario.terrenos = terrenos;
	escenario.protagonista=protagonista;
	
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

bool ParserYAML::validarScalarNumericoPositivo(const YAML::Node & node, std::string tag, unsigned int & salida) {
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

bool ParserYAML::validarScalarNumericoPositivo(const YAML::Node & node, std::string tag, double & salida) {
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

bool ParserYAML::validarScalarAlfaNumerico(const YAML::Node & node, std::string tag, std::string & salida) {
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


