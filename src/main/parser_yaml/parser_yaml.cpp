#include "parser_yaml.h"

const std::string NOMBRE_DEFAULT = "ENTIDAD";
const std::string IMAGEN_DEFAULT = "IMAGEN";
const int ANCHO_BASE_DEFAULT = 0;
const int ALTO_BASE_DEFAULT = 0;
const int PIXELX_REF_DEFAULT = 0;
const int PIXELY_REF_DEFAULT = 0;
const int FPS_DEFAULT = 0;
const int DELAY_DEFAULT = 0;

ParserYAML::ParserYAML(std::string filename){
	this->filename = filename;
}

void ParserYAML::parse(){
	std::ifstream fin(this->filename.c_str());
	if(!fin){
		throw runtime_error("yaml-cpp: el archivo de configuracion no existe o bien no pudo ser abierto.");
	}
	if (fin.peek() == std::ifstream::traits_type::eof()) {
		throw runtime_error("yaml-cpp: el archivo de configuracion esta vacio.");
	} 
	try{
		YAML::Parser parser(fin);
		parser.GetNextDocument(this->doc);
		if(fin.is_open())
			fin.close();
		if(this->doc.Type() != YAML::NodeType::Map) {
			throw runtime_error("yaml-cpp: el archivo de configuracion no tiene la estructura correcta o no existe.");
		}
	}
	catch(YAML::ParserException &e){ 
		std::string message = std::string(e.what());
		if(fin.is_open())
			fin.close();
		throw runtime_error(message.c_str());
	}
}

TagConfiguracion ParserYAML::getConfiguracion(){
	TagConfiguracion configuracion;
	if(this->doc.FindValue("configuracion") != NULL){
		const YAML::Node& conf = this->doc["configuracion"];
		if(conf.Type() == YAML::NodeType::Sequence){
			if(conf.size() == 1)
				setConfiguracion(conf[0],configuracion);
			else
				throw runtime_error("yaml-cpp: el tag de configuracion posee mas de un elemento en la secuencia.");
		}
		else
			throw runtime_error("yaml-cpp: el tag de configuracion no es del tipo Sequence.");
	}
	else
		throw runtime_error("yaml-cpp: el archivo de configuracion no tiene un tag configuracion.");
	return configuracion;
}

TagPantalla ParserYAML::getPantalla(){
	TagPantalla pantalla;
	if(this->doc.FindValue("pantalla") != NULL){
		const YAML::Node& pant = this->doc["pantalla"];
		if(pant.Type() == YAML::NodeType::Sequence){
			if(pant.size() == 1)
				setPantalla(pant[0], pantalla);
			else
				throw runtime_error("yaml-cpp: el tag de pantalla posee mas de un elemento en la secuencia.");
		}
		else
			throw runtime_error("yaml-cpp: el tag de pantalla no es del tipo Sequence.");
	}
	else
		throw runtime_error("yaml-cpp: el archivo de configuracion no tiene un tag pantalla.");
	return pantalla;
}

std::vector<TagTipoEntidad> ParserYAML::getTiposEntidades(){
	std::vector<TagTipoEntidad> tiposDeEntidades;
	if(this->doc.FindValue("tipos") != NULL){
		const YAML::Node& tipos = this->doc["tipos"];
		if(tipos.Type() == YAML::NodeType::Sequence){
			for(unsigned int i = 0; i < tipos.size(); i++){
				TagTipoEntidad tipoEntidad;
				setTipoEntidad(tipos[i], tipoEntidad);
				tiposDeEntidades.push_back(tipoEntidad);
			}
		}
		else
			throw runtime_error("yaml-cpp: el tag de tipos no es del tipo Sequence.");
	}
	else
		throw runtime_error("yaml-cpp: el archivo de configuracion no tiene una configuracion de los tipos de entidades.");
	return tiposDeEntidades;
}

TagEscenario ParserYAML::getEscenario(){
	TagEscenario escenario;
	if(this->doc.FindValue("escenario") != NULL){ 
		const YAML::Node& esc = this->doc["escenario"];
		if(esc.Type() == YAML::NodeType::Sequence){
			if(esc.size() == 1)
				setEscenario(esc[0],escenario);
			else
				throw runtime_error("yaml-cpp: el tag de escenario posee mas de un elemento en la secuencia.");
		}
		else
			throw runtime_error("yaml-cpp: el tag de escenario no es del tipo Sequence.");
	}
	else
		throw runtime_error("yaml-cpp: el archivo de configuracion no tiene una configuracion del escenario.");	
	return escenario;
}

ParserYAML::~ParserYAML(void){

}

void ParserYAML::setConfiguracion (const YAML::Node& node, TagConfiguracion& configuracion){
	if(node.Type() == YAML::NodeType::Map){
		if(!validarScalarNumericoPositivo(node, "vel_personaje", configuracion.vel_personaje)){
			// Poner valor por default
		}
		if(!validarScalarNumericoPositivo(node, "margen_scroll", configuracion.margen_scroll)){
			// Poner valor por default
		}
	}
	//else
	//	No es Map
		
}


void ParserYAML::setPantalla (const YAML::Node& node, TagPantalla& pantalla){
	if(node.Type() == YAML::NodeType::Map){
		if(!validarScalarNumericoPositivo(node, "ancho", pantalla.ancho)){
			// Poner valor por default
		}
		if(!validarScalarNumericoPositivo(node, "alto", pantalla.alto)){
			// Poner valor por default
		}
	}
	//else
	//	No es Map
		
}

void ParserYAML::setTipoEntidad (const YAML::Node& node, TagTipoEntidad& tipoEntidad){
	if(node.Type() == YAML::NodeType::Map){
		if(!validarScalarAlfaNumerico(node, "nombre", tipoEntidad.nombre))
			tipoEntidad.nombre = NOMBRE_DEFAULT;

		if(!validarScalarAlfaNumerico(node, "imagen", tipoEntidad.imagen))
			tipoEntidad.imagen = IMAGEN_DEFAULT;

		if(!validarScalarNumericoPositivo(node, "ancho_base", tipoEntidad.ancho_base))
			tipoEntidad.ancho_base = ANCHO_BASE_DEFAULT;

		if(!validarScalarNumericoPositivo(node, "alto_base", tipoEntidad.alto_base))
			tipoEntidad.alto_base = ALTO_BASE_DEFAULT;

		if(!validarScalarNumericoPositivo(node, "pixel_ref_x", tipoEntidad.pixel_ref_x))
			tipoEntidad.pixel_ref_x = PIXELX_REF_DEFAULT;

		if(!validarScalarNumericoPositivo(node, "pixel_ref_y", tipoEntidad.pixel_ref_y))
			tipoEntidad.pixel_ref_y = PIXELY_REF_DEFAULT;

		if(!validarScalarNumericoPositivo(node, "fps", tipoEntidad.fps))
			tipoEntidad.fps = FPS_DEFAULT;

		if(!validarScalarNumericoPositivo(node, "delay", tipoEntidad.delay))
			tipoEntidad.delay = DELAY_DEFAULT;
	}
	//else
	//	No es Map
}

void ParserYAML::setEntidad(const YAML::Node& node, TagEntidad& entidad){
	if(node.Type() == YAML::NodeType::Map){
		if(!validarScalarNumericoPositivo(node, "x", entidad.pos_x)){
			//Poner valor por defecto
		}

		if(!validarScalarNumericoPositivo(node, "y", entidad.pos_y)){
			//Poner valor por defecto
		}

		if(!validarScalarAlfaNumerico(node, "tipo", entidad.tipoEntidad)){
			//Poner valor por defecto
		}
	}
	//else
	//	No es map
}

void ParserYAML::setEscenario(const YAML::Node& node, TagEscenario& escenario){
	if(node.Type() == YAML::NodeType::Map){
		if(!validarScalarAlfaNumerico(node, "nombre", escenario.nombre)){
			//Poner valor por defecto
		}

		if(!validarScalarNumericoPositivo(node, "size_x", escenario.size_x)){
			//Poner valor por defecto
		}

		if(!validarScalarNumericoPositivo(node, "size_y", escenario.size_y)){
			//Poner valor por defecto
		}

		std::vector<TagEntidad> entidades;
		if(node.FindValue("entidades") != NULL){ 
			const YAML::Node& ent = node["entidades"];
			if(ent.Type() == YAML::NodeType::Sequence){
				for(unsigned int i = 0; i < ent.size(); i++){
					TagEntidad entidad;
					setEntidad(ent[i],entidad);
					entidades.push_back(entidad);
				}
			}
			else
				throw runtime_error("yaml-cpp: el tag de entidades del escenario no es del tipo Sequence.");
		}
		else
			throw runtime_error("yaml-cpp: el tag de entidades del escenario no existe.");
		escenario.entidades = entidades;

		TagEntidad protagonista;
		if(node.FindValue("protagonista") != NULL){ 
			const YAML::Node& pro = node["protagonista"];
			if(pro.Type() == YAML::NodeType::Sequence){
				if(pro.size() == 1){
					setEntidad(pro[0], protagonista);
					escenario.protagonista = protagonista;
				}
				else
					throw runtime_error("yaml-cpp: el tag de protagonista del escenario posee mas de un elemento en la secuencia.");
			}
			else
				throw runtime_error("yaml-cpp: el tag de protagonista del escenario no es del tipo Sequence.");
		}
		else
			throw runtime_error("yaml-cpp: el tag de protagonista del escenario no existe.");
	}
	//else
	//	No es Map
}

bool ParserYAML::esNumero(std::string numero) { 
	int i = 0;
	int largo = numero.length();
	if(numero.empty() || ((numero[0] != '-') && (!isdigit(numero[0])))){
		return false;
	}
	if(numero[0] == '-'){
		i = 1;
	}
	for(i; i < largo; i++) {
		if(numero[i] < '0' || numero[i] > '9') 
			return false; 
	}
	return true; 
}

bool ParserYAML::validarScalarNumericoPositivo(const YAML::Node & nodo, std::string tag, unsigned int & salida){
	std::string numero;
	int num;
	if(nodo.FindValue(tag)!= NULL) {
		const YAML::Node& nodo_tag = nodo[tag];
		if (nodo_tag.Type() == YAML::NodeType::Scalar ){
			nodo_tag >> numero;
			if (esNumero(numero)){
				nodo_tag >> num;
				if(num >= 0){
					nodo_tag >> salida;
					return true;
				}
				else{
					// El numero es negativo
				}
			}
			else{
				// El dato no es un numero
			}
		}
		else{
			// No es escalar el tag
		}
	}
	else{
		// No existe el tag
	}
	return false;
}

bool ParserYAML::validarScalarAlfaNumerico(const YAML::Node & nodo, std::string tag, std::string & salida){
	std::string cadena;
	if(nodo.FindValue(tag)!= NULL) {
		const YAML::Node& nodo_tag = nodo[tag];
		if (nodo_tag.Type() == YAML::NodeType::Scalar ){
			nodo_tag >> salida;
			return true;
		}
		else{
			// No es escalar el tag
		}
	}
	else{
		// No existe el tag
	}
	return false;
}

std::string ubicarNodo(const YAML::Mark marca_nodo){
	std::stringstream salida;
    salida << "linea " << marca_nodo.line+1 << ", columna " << marca_nodo.column+1 << " ";
	return salida.str();
}

