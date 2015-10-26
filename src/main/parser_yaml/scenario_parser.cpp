#include "scenario_parser.h"

using namespace std;

ScenarioParser::ScenarioParser(std::string filename, std::string filenameDefault) : GenericParser(filename, filenameDefault) {
}

ScenarioParser::~ScenarioParser(void) {
}

TagEscenario ScenarioParser::getEscenario() {
	Logger::getInstance()->writeInformation("YAML-CPP:Se obtiene informacion de escenario.");
	TagEscenario escenario;
	if (this->doc.FindValue("escenario")) {
		const YAML::Node& esc = this->doc["escenario"];
		if (esc.Type() == YAML::NodeType::Map) {
			setEscenario(esc, escenario);
		}
		else {
			Logger::getInstance()->writeWarning("YAML-CPP:El tag de escenario no es del tipo Map. Ubicar" + ubicarNodo(esc.GetMark()));
			setEscenarioDefault(escenario);
		}
	}
	else {
		Logger::getInstance()->writeWarning("YAML-CPP:El archivo de configuracion no tiene una configuracion del escenario.");
		setEscenarioDefault(escenario);
	}
	return escenario;
}

void ScenarioParser::setEntidad(const YAML::Node& node, TagEntidad& entidad) {
	if (node.Type() == YAML::NodeType::Map) {
		if (!obtenerValorScalarNumericoPositivo(node, "x", entidad.pos.x)) {
			Logger::getInstance()->writeWarning("YAML-CPP:La posicion x de la entidad se toma por defecto.");
			entidad.pos.x = ENTIDAD_DEFAULT_POSX;
		}

		if (!obtenerValorScalarNumericoPositivo(node, "y", entidad.pos.y)) {
			Logger::getInstance()->writeWarning("YAML-CPP:La posicion y de la entidad se toma por defecto.");
			entidad.pos.y = ENTIDAD_DEFAULT_POSY;
		}

		if ((!obtenerValorScalarAlfaNumerico(node, "tipo", entidad.tipoEntidad)) || (entidad.tipoEntidad.empty())) {
			Logger::getInstance()->writeWarning("YAML-CPP:El tipo de la entidad se toma por defecto.");
			entidad.tipoEntidad = ENTIDAD_DEFAULT_NOMBRE;
		}
	}
	else {
		Logger::getInstance()->writeWarning("YAML-CPP:El contenido de la entidad no es del tipo Map. Ubicar" + ubicarNodo(node.GetMark()));
		Logger::getInstance()->writeInformation("YAML-CPP:Se toman valores por default para esa entidad");
		entidad.tipoEntidad = ENTIDAD_DEFAULT_NOMBRE;
		entidad.pos.y = ENTIDAD_DEFAULT_POSY;
		entidad.pos.x = ENTIDAD_DEFAULT_POSX;
	}
}

void ScenarioParser::setEscenario(const YAML::Node& node, TagEscenario& escenario) {
	if (node.Type() == YAML::NodeType::Map) {
		if (!obtenerValorScalarAlfaNumerico(node, "nombre", escenario.nombre)) {
			Logger::getInstance()->writeWarning("YAML-CPP:El nombre del escenario se toma por default.");
			escenario.nombre = ESCENARIO_DEFAULT_NOMBRE;
		}

		if ((!obtenerValorScalarNumericoPositivo(node, "size_x", escenario.size_x)) || (!obtenerValorScalarNumericoPositivo(node, "size_y", escenario.size_y))) {
			Logger::getInstance()->writeInformation("YAML-CPP:Se toman por default los datos del tamanio del escenario.");
			escenario.size_x = ESCENARIO_DEFAULT_SIZE_X;
			escenario.size_y = ESCENARIO_DEFAULT_SIZE_Y;
		}
		if (!obtenerValorScalarNumericoPositivo(node, "max_resources", escenario.max_resources)) {
			Logger::getInstance()->writeWarning("YAML-CPP:Max_resources del escenario se toma por default.");
			escenario.max_resources = ESCENARIO_DEFAULT_MAXRESOURCES;
		}

		std::vector<TagEntidad> entidades;
		if (node.FindValue("entidades")) {
			const YAML::Node& ent = node["entidades"];
			if (ent.Type() == YAML::NodeType::Sequence) {
				for (unsigned int i = 0; i < ent.size(); i++) {
					TagEntidad entidad;
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
		escenario.entidades = entidades;

		std::vector<TagEntidad> terrenos;
		if (node.FindValue("terrenos")) {
			const YAML::Node& ter = node["terrenos"];
			if (ter.Type() == YAML::NodeType::Sequence) {
				for (unsigned int i = 0; i < ter.size(); i++) {
					TagEntidad terreno;
					setEntidad(ter[i], terreno);
					terrenos.push_back(terreno);
				}
			}
			else {
				Logger::getInstance()->writeWarning("YAML-CPP:El tag terrenos no es del tipo secuencia.");
				Logger::getInstance()->writeInformation("YAML-CPP: No se toman terrenos.");
			}
		}
		else {
			Logger::getInstance()->writeWarning("YAML-CPP:El tag terrenos no existe en el archivo.");
			Logger::getInstance()->writeInformation("YAML-CPP: No se toman terrenos.");
		}
		escenario.terrenos = terrenos;

		//TagEntidad protagonista;
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
		std::vector<TagJugador> jugadores;
		if (node.FindValue("jugadores")) {
			const YAML::Node& jugs = node["jugadores"];
			if (jugs.Type() == YAML::NodeType::Sequence) {
				for (unsigned int i = 0; i < jugs.size(); i++) {
					TagJugador jugador;
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
	else {
		Logger::getInstance()->writeWarning("YAML-CPP: El tag de escenario no es del tipo Map. Ubicar:" + ubicarNodo(node.GetMark()));
		setEscenarioDefault(escenario);
	}
}

void ScenarioParser::setJugador(const YAML::Node& node, TagJugador& jugador, int i) {
	if (node.Type() == YAML::NodeType::Map) {
		if (!obtenerValorScalarAlfaNumerico(node, "name", jugador.name)) {
			Logger::getInstance()->writeWarning("YAML-CPP:El nombre del jugador se toma por defecto.");
			jugador.name = DEFAULT_PLAYER_NAME + intToString(i);
		}

		std::string isHuman;
		if (!obtenerValorScalarAlfaNumerico(node, "isHuman", isHuman) || (isHuman != "true")) {
			Logger::getInstance()->writeWarning("YAML-CPP:El isHuman del jugador se toma por defecto false.");
			jugador.isHuman = false;
		}
		else
			jugador.isHuman = true;

		std::vector<TagEntidad> entidades;
		if (node.FindValue("entidades")) {
			const YAML::Node& ent = node["entidades"];
			if (ent.Type() == YAML::NodeType::Sequence) {
				for (unsigned int i = 0; i < ent.size(); i++) {
					TagEntidad entidad;
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
		std::vector<TagEntidad> entitiesDefault;
		jugador.entidades = entitiesDefault;
	}
}

void ScenarioParser::setProtagonistaDefault(TagEntidad& protagonista) {
	Logger::getInstance()->writeWarning("YAML-CPP: Se toma protagonista por default.");
	protagonista.tipoEntidad = PROTAGONISTA_DEFAULT_NOMBRE;
	protagonista.pos.x = PROTAGONISTA_DEFAULT_POSX;
	protagonista.pos.y = PROTAGONISTA_DEFAULT_POSY;
}

void ScenarioParser::setEscenarioDefault(TagEscenario& escenario) {
	Logger::getInstance()->writeWarning("YAML-CPP: Se toma escenario por default.");
	std::vector<TagEntidad> entidades;
	std::vector<TagEntidad> terrenos;
	std::vector<TagJugador> jugadores;

	//TagEntidad protagonista;
	//escenario.protagonista = protagonista;
	//setProtagonistaDefault(protagonista);

	escenario.nombre = ESCENARIO_DEFAULT_NOMBRE;
	escenario.size_x = ESCENARIO_DEFAULT_SIZE_X;
	escenario.size_y = ESCENARIO_DEFAULT_SIZE_Y;
	escenario.max_resources = ESCENARIO_DEFAULT_MAXRESOURCES;
	escenario.entidades = entidades;
	escenario.terrenos = terrenos;
	escenario.jugadores = jugadores;
}
