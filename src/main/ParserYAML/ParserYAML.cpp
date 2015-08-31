#include "ParserYAML.h"

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
		throw exception("yaml-cpp: el archivo de configuracion no existe o bien no pudo ser abierto.");
	}
	if (fin.peek() == std::ifstream::traits_type::eof()) {
		throw exception("yaml-cpp: el archivo de configuracion esta vacio.");
	} 
	try{
		YAML::Parser parser(fin);
		parser.GetNextDocument(this->doc);
		if(fin.is_open())
			fin.close();
		if(this->doc.Type() != YAML::NodeType::Map) {
			throw exception("yaml-cpp: el archivo de configuracion no tiene la estructura correcta o no existe.");
		}
	}
	catch(YAML::ParserException &e){ 
		std::string message = std::string(e.what());
		if(fin.is_open())
			fin.close();
		throw exception(message.c_str());
	}
}

TagConfiguracion ParserYAML::getConfiguracion(){
	TagConfiguracion configuracion;
	const YAML::Node& conf = this->doc["configuracion"];
	if(conf.size() != 1)
		throw exception("yaml-cpp: el archivo de configuracion tiene mas de una  configuracion o no existe.");
	setConfiguracion(conf[0],configuracion);
	return configuracion;
}

TagPantalla ParserYAML::getPantalla(){
	TagPantalla pantalla;
	const YAML::Node& pant = this->doc["pantalla"];
	if(pant.size() != 1)
		throw exception("yaml-cpp: el archivo de configuracion tiene mas de una configuracion de la pantalla o no existe.");
	setPantalla(pant[0], pantalla);
	return pantalla;
}

std::vector<TagTipoEntidad> ParserYAML::getTiposEntidades(){
	std::vector<TagTipoEntidad> tiposDeEntidades;
	const YAML::Node& tipos = this->doc["tipos"];
	for(unsigned int i = 0; i < tipos.size(); i++){
		TagTipoEntidad tipoEntidad;
		setTipoEntidad(tipos[i], tipoEntidad);
		tiposDeEntidades.push_back(tipoEntidad);
	}
	return tiposDeEntidades;
}

TagEscenario ParserYAML::getEscenario(){
	TagEscenario escenario;
	const YAML::Node& esc = this->doc["escenario"];
	if(esc.size() != 1)
		throw exception("yaml-cpp: el archivo de configuracion tiene mas de una configuracion del escenario o no existe.");
	setEscenario(esc[0],escenario);
	return escenario;
}

ParserYAML::~ParserYAML(void){

}

void ParserYAML::setConfiguracion (const YAML::Node& node, TagConfiguracion& configuracion){
	node["vel_personaje"] >> configuracion.vel_personaje;
	node["margen_scroll"] >> configuracion.margen_scroll;
}


void ParserYAML::setPantalla (const YAML::Node& node, TagPantalla& pantalla){
	node["ancho"] >> pantalla.ancho;
	node["alto"] >> pantalla.alto;
}

void ParserYAML::setTipoEntidad (const YAML::Node& node, TagTipoEntidad& tipoEntidad){
	if(node.FindValue("nombre") != NULL) node["nombre"] >> tipoEntidad.nombre; 
	else tipoEntidad.nombre = NOMBRE_DEFAULT;

	if(node.FindValue("imagen") != NULL) node["imagen"] >> tipoEntidad.imagen;
	else tipoEntidad.imagen = IMAGEN_DEFAULT;

	if(node.FindValue("ancho_base") != NULL)node["ancho_base"] >> tipoEntidad.ancho_base;
	else tipoEntidad.ancho_base = ANCHO_BASE_DEFAULT;

	if(node.FindValue("alto_base") != NULL) node["alto_base"] >> tipoEntidad.alto_base;
	else tipoEntidad.alto_base = ALTO_BASE_DEFAULT;

	if(node.FindValue("pixel_ref_x") != NULL) node["pixel_ref_x"] >> tipoEntidad.pixel_ref_x;
	else tipoEntidad.pixel_ref_x = PIXELX_REF_DEFAULT;

	if(node.FindValue("pixel_ref_y") != NULL) node["pixel_ref_y"] >> tipoEntidad.pixel_ref_y;
	else tipoEntidad.pixel_ref_y = PIXELY_REF_DEFAULT;

    if(node.FindValue("fps") != NULL) node["fps"] >> tipoEntidad.fps;
	else tipoEntidad.fps = FPS_DEFAULT;

	if(node.FindValue("delay") != NULL) node["delay"] >> tipoEntidad.delay;
	else tipoEntidad.delay = DELAY_DEFAULT;
}

void ParserYAML::setEntidad (const YAML::Node& node, TagEntidad& entidad){
	node["x"] >> entidad.pos_x;
	node["y"] >> entidad.pos_y;
	node["tipo"] >> entidad.tipoEntidad;
}

void ParserYAML::setEscenario(const YAML::Node& node, TagEscenario& escenario){
	node["nombre"] >> escenario.nombre;
	node["size_x"] >> escenario.size_x;
	node["size_y"] >> escenario.size_y;
	std::vector<TagEntidad> entidades;
	const YAML::Node& ent = node["entidades"];
	for(unsigned int i = 0; i < ent.size(); i++){
		TagEntidad entidad;
		setEntidad(ent[i],entidad);
		entidades.push_back(entidad);
	}
	escenario.entidades = entidades;
	TagEntidad protagonista;
    const YAML::Node& pro = node["protagonista"];
	if(pro.size() != 1)
		throw exception("yaml-cpp: el archivo de configuracion tiene mas de un protagonista o no tiene.");
	setEntidad(pro[0], protagonista);
	escenario.protagonista = protagonista;
}

bool ParserYAML::esNumero(std::string s) { 
	int i = 0;
	int longitud = s.length();
	if(s.empty() || ((s[0] != '-') && (!isdigit(s[0])))){
		return false;
	}
	if(s[0] == '-'){
		i = 1;
	}
	for(i; i < longitud; i++) {
		if(s[i] < '0' || s[i] > '9') 
			return false; 
	}
	return true; 
}

