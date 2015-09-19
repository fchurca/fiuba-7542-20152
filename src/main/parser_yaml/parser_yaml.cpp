#include "parser_yaml.h"


ParserYAML::ParserYAML(std::string filename) {
	this->filename = filename;
}

void ParserYAML::parse() {
	Logger::getInstance()->writeInformation("yaml-cpp: inicia parseo del archivo de configuracion.");
	std::ifstream fin(this->filename.c_str());
	if(!fin) {
		Logger::getInstance()->writeError("yaml-cpp: el archivo de configuracion no existe o bien no pudo ser abierto.");
		Logger::getInstance()->writeInformation("yaml-cpp: se toma un archivo de escenario default.");
		setArchivoDefault();
	}
	else if (fin.peek() == std::ifstream::traits_type::eof()) {
		Logger::getInstance()->writeError("yaml-cpp: el archivo de configuracion se encuentra vacio.");
		Logger::getInstance()->writeInformation("yaml-cpp: se toma un archivo de escenario default.");
		setArchivoDefault();
	} 
	else {
		try{
			YAML::Parser parser(fin);
			parser.GetNextDocument(this->doc);
			if(fin.is_open())
				fin.close();
			if(this->doc.Type() != YAML::NodeType::Map) {
				Logger::getInstance()->writeError("yaml-cpp: el archivo de configuracion no tiene la estructura correcta.");
				Logger::getInstance()->writeInformation("yaml-cpp: se toma un archivo de escenario default.");
				setArchivoDefault();
			}
		}
		catch(YAML::ParserException &e) { 
			std::string mensaje = std::string(e.what());
			if(fin.is_open())
				fin.close();
			Logger::getInstance()->writeError("yaml-cpp:" + mensaje);
			Logger::getInstance()->writeInformation("yaml-cpp: se toma un archivo de escenario default.");
			setArchivoDefault();
		}
	}
}

TagConfiguracion ParserYAML::getConfiguracion() {
	Logger::getInstance()->writeInformation("yaml-cpp: se obtiene la informacion de configuracion.");
	TagConfiguracion configuracion;
	if(this->doc.FindValue("configuracion")) {
		const YAML::Node& conf = this->doc["configuracion"];
		if(conf.Type() == YAML::NodeType::Map) {
			setConfiguracion(conf,configuracion);
		}
		else{
			Logger::getInstance()->writeWarning("yaml-cpp:el tag de configuracion no es del tipo Map. Ubicar" + ubicarNodo(conf.GetMark()));
			setConfiguracionDefault(configuracion);
		}
	}
	else{
		Logger::getInstance()->writeWarning("yaml-cpp:el tag de configuracion no existe en el archivo.");
		setConfiguracionDefault(configuracion);
	}
	return configuracion;
}

TagPantalla ParserYAML::getPantalla() {
	Logger::getInstance()->writeInformation("yaml-cpp: se obtiene la informacion de la pantalla.");
	TagPantalla pantalla;
	if(this->doc.FindValue("pantalla")) {
		const YAML::Node& pant = this->doc["pantalla"];
		if(pant.Type() == YAML::NodeType::Map) {
			setPantalla(pant,pantalla);
		}
		else{
			Logger::getInstance()->writeWarning("yaml-cpp:el tag de pantalla no es del tipo Map. Ubicar" + ubicarNodo(pant.GetMark()));
			setPantallaDefault(pantalla);
		}
	}
	else{
		Logger::getInstance()->writeWarning("yaml-cpp:el tag de pantalla no existe en el archivo.");
		setPantallaDefault(pantalla);
	}
	return pantalla;
}

std::vector<TagTipoEntidad> ParserYAML::getTiposEntidades() {
	Logger::getInstance()->writeInformation("yaml-cpp: se obtiene informacion de los tipos de entidades.");
	std::vector<TagTipoEntidad> tiposDeEntidades;
	if(this->doc.FindValue("tipos")) {
		const YAML::Node& tipos = this->doc["tipos"];
		if(tipos.Type() == YAML::NodeType::Sequence) {
			for(unsigned int i = 0; i < tipos.size(); i++) {
				Logger::getInstance()->writeInformation("yaml-cpp: se obtiene informacion del tipo de entidad numero."  + intToString(i));
				TagTipoEntidad tipoEntidad;
				setTipoEntidad(tipos[i], tipoEntidad);
				tiposDeEntidades.push_back(tipoEntidad);
			}
		}
		else{
			Logger::getInstance()->writeWarning("yaml-cpp:el tag de tipos no es del tipo Sequence. Ubicar" + ubicarNodo(tipos.GetMark()));
			Logger::getInstance()->writeInformation("yaml-cpp: se toman tipos de entidad por default");
			TagTipoEntidad tipoEntidadDefault;
			setTipoEntidadDefault(tipoEntidadDefault);
			tiposDeEntidades.push_back(tipoEntidadDefault);
		}
	}
	else{
		Logger::getInstance()->writeWarning("yaml-cpp:el tag de tipos no existe en el archivo.");
		Logger::getInstance()->writeInformation("yaml-cpp: se toman tipos de entidad por default");
		TagTipoEntidad tipoEntidadDefault;
		setTipoEntidadDefault(tipoEntidadDefault);
		tiposDeEntidades.push_back(tipoEntidadDefault);
	}
	return tiposDeEntidades;
}

TagEscenario ParserYAML::getEscenario() {
	Logger::getInstance()->writeInformation("yaml-cpp: se obtiene informacion de escenario.");
	TagEscenario escenario;
	if(this->doc.FindValue("escenario")) { 
		const YAML::Node& esc = this->doc["escenario"];
		if(esc.Type() == YAML::NodeType::Map) {
			setEscenario(esc,escenario);
		}
		else{
			Logger::getInstance()->writeWarning("yaml-cpp:el tag de escenario no es del tipo Map. Ubicar" + ubicarNodo(esc.GetMark()));
			setEscenarioDefault(escenario);
		}
	}
	else{
		Logger::getInstance()->writeWarning("yaml-cpp: el archivo de configuracion no tiene una configuracion del escenario.");	
		setEscenarioDefault(escenario);
	}
	return escenario;
}

ParserYAML::~ParserYAML(void) {

}

void ParserYAML::setConfiguracion (const YAML::Node& node, TagConfiguracion& configuracion) {
	if(node.Type() == YAML::NodeType::Map) {
		if(!validarScalarNumericoPositivo(node, "vel_personaje", configuracion.vel_personaje))
			configuracion.vel_personaje = VELOCIDAD_PERSONAJE_DEFAULT;
		if(!validarScalarNumericoPositivo(node, "margen_scroll", configuracion.margen_scroll))
			configuracion.margen_scroll = MARGEN_SCROLL_DEFAULT;
		if(!validarScalarNumericoPositivo(node, "velocidad_scroll", configuracion.velocidad_scroll))
			configuracion.velocidad_scroll = VELOCIDAD_SCROLL_DEFAULT;
	}
	else{
		Logger::getInstance()->writeWarning("yaml-cpp:el contenido del tag de configuracion no es del tipo Map. Ubicar" + ubicarNodo(node.GetMark()));
		setConfiguracionDefault(configuracion);
	}	
}

void ParserYAML::setConfiguracionDefault (TagConfiguracion& configuracion) {
	Logger::getInstance()->writeInformation("yaml-cpp: se toma configuracion por default.");
	configuracion.margen_scroll = MARGEN_SCROLL_DEFAULT;
	configuracion.margen_scroll = VELOCIDAD_PERSONAJE_DEFAULT;
	configuracion.velocidad_scroll = VELOCIDAD_SCROLL_DEFAULT;
}


void ParserYAML::setPantalla (const YAML::Node& node, TagPantalla& pantalla) {
	if(node.Type() == YAML::NodeType::Map) {
		if((!validarScalarNumericoPositivo(node, "ancho", pantalla.ancho)) || (!validarScalarNumericoPositivo(node, "alto", pantalla.alto)))
			setPantallaDefault(pantalla);	
	}
	else{
		Logger::getInstance()->writeWarning("yaml-cpp:el contenido del tag de pantalla no es del tipo Map. Ubicar" + ubicarNodo(node.GetMark()));
		setPantallaDefault(pantalla);
	}	
}

void ParserYAML::setPantallaDefault (TagPantalla& pantalla) {
	Logger::getInstance()->writeInformation("yaml-cpp: se toma pantalla por default.");
	pantalla.alto = ALTO_DEFAULT;
	pantalla.ancho = ANCHO_DEFAULT;
}

void ParserYAML::setTipoEntidad (const YAML::Node& node, TagTipoEntidad& tipoEntidad) {
	if(node.Type() == YAML::NodeType::Map) {
		if(!validarScalarAlfaNumerico(node, "nombre", tipoEntidad.nombre)) {
			Logger::getInstance()->writeInformation("yaml-cpp: el nombre del tipo de entidad se toma por default.");
			tipoEntidad.nombre = NOMBRE_TIPO_ENTIDAD_DEFAULT;
		}
		if((!validarScalarAlfaNumerico(node, "imagen", tipoEntidad.imagen))
			|| (!validarScalarNumericoPositivo(node, "ancho_base", tipoEntidad.ancho_base))
			|| (!validarScalarNumericoPositivo(node, "alto_base", tipoEntidad.alto_base))
			|| (!validarScalarNumericoPositivo(node, "pixel_ref_x", tipoEntidad.pixel_ref_x))
			|| (!validarScalarNumericoPositivo(node, "pixel_ref_y", tipoEntidad.pixel_ref_y))
			|| (!validarScalarNumericoPositivo(node, "ancho_sprite", tipoEntidad.ancho_sprite))
			|| (!validarScalarNumericoPositivo(node, "alto_sprite", tipoEntidad.alto_sprite))
			|| (!validarScalarNumericoPositivo(node, "cantidad_sprites", tipoEntidad.cantidad_sprites))) {
				Logger::getInstance()->writeWarning("yaml-cpp: datos de la imagen del tipo de entidad invalidos, se toman por default (path, ancho_base, alto_base, pixel_ref_x, pixel_ref_y).");
				tipoEntidad.imagen = IMAGEN_TIPO_ENTIDAD_DEFAULT;
				tipoEntidad.ancho_base = ANCHO_BASE_DEFAULT;
				tipoEntidad.alto_base = ALTO_BASE_DEFAULT;
				tipoEntidad.pixel_ref_x = PIXELX_REF_DEFAULT;
				tipoEntidad.pixel_ref_y = PIXELY_REF_DEFAULT;
				tipoEntidad.alto_sprite = ALTO_SPRITE_DEFAULT;
				tipoEntidad.ancho_sprite = ANCHO_SPRITE_DEFAULT;
				tipoEntidad.cantidad_sprites = CANTIDAD_SPRITES_DEFAULT;
		}
		if(!validarScalarNumericoPositivo(node, "fps", tipoEntidad.fps)){
			Logger::getInstance()->writeWarning("yaml-cpp: Se toma por default (fps).");
			tipoEntidad.fps = FPS_DEFAULT;
		}
		if(!validarScalarNumericoPositivo(node, "delay", tipoEntidad.delay)){
			Logger::getInstance()->writeWarning("yaml-cpp: Se toma por default (delay).");
			tipoEntidad.delay = DELAY_DEFAULT;
		}
	}
	else{
		Logger::getInstance()->writeWarning("yaml-cpp:el contenido del tipo de entidad no es del tipo Map. Ubicar" + ubicarNodo(node.GetMark()));
		setTipoEntidadDefault(tipoEntidad);
	}
}

void ParserYAML::setTipoEntidadDefault (TagTipoEntidad& tipoEntidad) {
	Logger::getInstance()->writeInformation("yaml-cpp: se toma tipo de entidad por default.");
	tipoEntidad.nombre = NOMBRE_TIPO_ENTIDAD_DEFAULT;
	tipoEntidad.imagen = IMAGEN_TIPO_ENTIDAD_DEFAULT;
	tipoEntidad.ancho_base = ANCHO_BASE_DEFAULT;
	tipoEntidad.alto_base = ALTO_BASE_DEFAULT;
	tipoEntidad.pixel_ref_x = PIXELX_REF_DEFAULT;
	tipoEntidad.pixel_ref_y = PIXELY_REF_DEFAULT;
	tipoEntidad.fps = FPS_DEFAULT;
	tipoEntidad.delay = DELAY_DEFAULT;
	tipoEntidad.alto_sprite = ALTO_SPRITE_DEFAULT;
	tipoEntidad.ancho_sprite = ANCHO_SPRITE_DEFAULT;
	tipoEntidad.cantidad_sprites = CANTIDAD_SPRITES_DEFAULT;
}

void ParserYAML::setEntidad(const YAML::Node& node, TagEntidad& entidad) {
	if(node.Type() == YAML::NodeType::Map) {
		if(!validarScalarNumericoPositivo(node, "x", entidad.pos_x)) {
			Logger::getInstance()->writeInformation("yaml-cpp: la posicion x de la entidad se toma por defecto.");
			entidad.pos_x = POS_X_DEFAULT;
		}

		if(!validarScalarNumericoPositivo(node, "y", entidad.pos_y)) {
			Logger::getInstance()->writeInformation("yaml-cpp: la posicion y de la entidad se toma por defecto.");
			entidad.pos_y = POS_Y_DEFAULT;
		}

		if(!validarScalarAlfaNumerico(node, "tipo", entidad.tipoEntidad)) {
			Logger::getInstance()->writeInformation("yaml-cpp: el tipo de la entidad se toma por defecto.");
			entidad.tipoEntidad = TIPO_DEFAULT;
		}
	}
	else{
		Logger::getInstance()->writeWarning("yaml-cpp:el contenido de la entidad no es del tipo Map. Ubicar" + ubicarNodo(node.GetMark()));
		Logger::getInstance()->writeInformation("yaml-cpp: se toman valores por default para esa entidad");
		entidad.tipoEntidad = TIPO_DEFAULT;
		entidad.pos_y = POS_Y_DEFAULT;
		entidad.pos_x = POS_X_DEFAULT;
	}
}

void ParserYAML::setEscenario(const YAML::Node& node, TagEscenario& escenario) {
	if(node.Type() == YAML::NodeType::Map) {
		if(!validarScalarAlfaNumerico(node, "nombre", escenario.nombre)) {
			Logger::getInstance()->writeInformation("yaml-cpp: el nombre del escenario se toma por default.");
			escenario.nombre = NOMBRE_ESCENARIO_DEFAULT;
		}

		if((!validarScalarNumericoPositivo(node, "size_x", escenario.size_x))||(!validarScalarNumericoPositivo(node, "size_y", escenario.size_y))) {
			Logger::getInstance()->writeInformation("yaml-cpp: se toman por default los datos del tamonio del escenario.");
			escenario.size_x = SIZE_X_DEFAULT;
			escenario.size_x = SIZE_Y_DEFAULT;
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
				Logger::getInstance()->writeWarning("yaml-cpp: el tag entidades no es del tipo secuencia.");
				Logger::getInstance()->writeInformation("yaml-cpp: No se toman entidades.");
			}
		}
		else{
			Logger::getInstance()->writeWarning("yaml-cpp: el tag entidades no existe en el archivo.");
			Logger::getInstance()->writeInformation("yaml-cpp: No se toman entidades.");
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
				Logger::getInstance()->writeWarning("yaml-cpp: el tag terrenos no es del tipo secuencia.");
				Logger::getInstance()->writeInformation("yaml-cpp: No se toman terrenos.");
			}
		}
		else{
			Logger::getInstance()->writeWarning("yaml-cpp: el tag terrenos no existe en el archivo.");
			Logger::getInstance()->writeInformation("yaml-cpp: No se toman terrenos.");
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
					Logger::getInstance()->writeWarning("yaml-cpp: el tag protagonista posee mas de un elemento. Ubicar:" + ubicarNodo(pro.GetMark()));
				}
			}
			else{
				Logger::getInstance()->writeWarning("yaml-cpp: el tag de protagonista del escenario no es del tipo Sequence. Ubicar:" + ubicarNodo(pro.GetMark()));
			}
		}
		else{
			Logger::getInstance()->writeWarning("yaml-cpp: el tag de protagonista del escenario no existe.");
		}
	}
	else{
		Logger::getInstance()->writeWarning("yaml-cpp: el tag de escenario no es del tipo Map. Ubicar:" + ubicarNodo(node.GetMark()));
		setEscenarioDefault(escenario);
	}
		
}

void ParserYAML::setEscenarioDefault (TagEscenario& escenario) {
	Logger::getInstance()->writeInformation("yaml-cpp: se toma escenario por default.");
	std::vector<TagEntidad> entidades;
	std::vector<TagEntidad> terrenos;
	TagEntidad protagonista;
	escenario.nombre = NOMBRE_ESCENARIO_DEFAULT;
	escenario.size_x = SIZE_X_DEFAULT;
	escenario.size_y= SIZE_Y_DEFAULT;
	escenario.entidades = entidades;
	escenario.terrenos = terrenos;

	
}

void ParserYAML::setArchivoDefault() {
	// Este metodo no puede fallar, el archivo default no puede contener errores.
	this->filename = ARCHIVO_CONFIGURACION_DEFAULT;
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
		i = 1;
	}
	for(; i < largo; i++){
		if((numero[i] < '0' || numero[i] > '9') && numero[i] != '.') 
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
						Logger::getInstance()->writeWarning("yaml-cpp:el valor del tag: " + tag + " es un numero decimal. Ubicar" + ubicarNodo(nodo_tag.GetMark()));
				}
				else
					Logger::getInstance()->writeWarning("yaml-cpp:el valor del tag: " + tag + " es un numero negativo. Ubicar" + ubicarNodo(nodo_tag.GetMark()));
			}
			else
				Logger::getInstance()->writeWarning("yaml-cpp:el valor del tag: " + tag + " no es un numero. Ubicar" + ubicarNodo(nodo_tag.GetMark()));
		}
		else
			Logger::getInstance()->writeWarning("yaml-cpp:el valor del tag: " + tag + " no es del tipo Scalar. Ubicar" + ubicarNodo(nodo_tag.GetMark()));
	}
	else
		Logger::getInstance()->writeWarning("yaml-cpp:el tag: " + tag + " no existe en el nodo. Ubicar" + ubicarNodo(node.GetMark()));
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
					Logger::getInstance()->writeWarning("yaml-cpp:el valor del tag: " + tag + " es un numero negativo. Ubicar" + ubicarNodo(nodo_tag.GetMark()));
			}
			else
				Logger::getInstance()->writeWarning("yaml-cpp:el valor del tag: " + tag + " no es un numero. Ubicar" + ubicarNodo(nodo_tag.GetMark()));
		}
		else
			Logger::getInstance()->writeWarning("yaml-cpp:el valor del tag: " + tag + " no es del tipo Scalar. Ubicar" + ubicarNodo(nodo_tag.GetMark()));
	}
	else
		Logger::getInstance()->writeWarning("yaml-cpp:el tag: " + tag + " no existe en el nodo. Ubicar" + ubicarNodo(node.GetMark()));
	return false;
}

bool ParserYAML::validarScalarAlfaNumerico(const YAML::Node & node, std::string tag, std::string & salida) {
	std::string cadena;
	if(node.FindValue(tag)) {
		const YAML::Node& nodo_tag = node[tag];
		if (nodo_tag.Type() == YAML::NodeType::Scalar) {
			nodo_tag >> salida;
			return true;
		}
		else
			Logger::getInstance()->writeWarning("yaml-cpp:el valor del tag: " + tag + " no es del tipo Scalar. Ubicar" + ubicarNodo(nodo_tag.GetMark()));
	}
	else
		Logger::getInstance()->writeWarning("yaml-cpp:el tag: " + tag + " no existe en el nodo. Ubicar" + ubicarNodo(node.GetMark()));
	return false;
}



std::string ParserYAML::ubicarNodo(const YAML::Mark marca_nodo) {
	std::stringstream salida;
    salida << "linea " << marca_nodo.line+1 << ", columna " << marca_nodo.column+1 << " ";
	return salida.str();
}

std::string ParserYAML::intToString(int i) {
	string resultado;          
	ostringstream aux;   
	aux << i;      
	resultado = aux.str();
	return resultado;
}


