#include "server_parser.h"

using namespace std;

ServerParser::ServerParser(std::string filename) : GenericParser(filename, SERVER_CONFIG_FILE_PATH_DEFAULT) {
}

ServerParser::~ServerParser(void) {

}


TagServerConfiguration ServerParser::getServerConfiguration() {
	//Logger::getInstance()->writeInformation("YAML-CPP:Se obtiene la informacion de configuracion.");
	TagServerConfiguration configuration;
	if (this->doc.FindValue("configuration")) {
		const YAML::Node& conf = this->doc["configuration"];
		if (conf.Type() == YAML::NodeType::Map) {
			setServerConfiguration(conf, configuration);
		}
		else {
			//Logger::getInstance()->writeWarning("YAML-CPP:El tag de configuracion no es del tipo Map. Ubicar" + ubicarNodo(conf.GetMark()));
			setServerConfigurationDefault(configuration);
		}
	}
	else {
		//Logger::getInstance()->writeWarning("YAML-CPP:El tag de configuracion no existe en el archivo.");
		setServerConfigurationDefault(configuration);
	}
	return configuration;
}

void ServerParser::setServerConfiguration(const YAML::Node& node, TagServerConfiguration& configuration) {
	if (node.Type() == YAML::NodeType::Map) {
		if (!obtenerValorScalarAlfaNumerico(node, "ip", configuration.ip)) {
			//Logger::getInstance()->writeWarning("YAML-CPP: Se toma por default (velocidad personaje).");
			configuration.ip = SERVER_IP_DEFAULT;
		}
		if (!obtenerValorScalarNumericoPositivo(node, "port", configuration.port)) {
			//Logger::getInstance()->writeWarning("YAML-CPP: Se toma por default (margen scroll).");
			configuration.port = SERVER_PORT_DEFAULT;
		}
		if (!obtenerValorScalarNumericoPositivo(node, "max_clients", configuration.max_clients)) {
			//Logger::getInstance()->writeWarning("YAML-CPP: Se toma por default (velocidad scroll).");
			configuration.max_clients = SERVER_MAX_CLIENTS_DEFAULT;
		}
	}
	else {
		//Logger::getInstance()->writeWarning("YAML-CPP:El contenido del tag de configuracion no es del tipo Map. Ubicar" + ubicarNodo(node.GetMark()));
		setServerConfigurationDefault(configuration);
	}
}

void ServerParser::setServerConfigurationDefault(TagServerConfiguration& configuration) {
	configuration.ip = SERVER_IP_DEFAULT;
	configuration.port = SERVER_PORT_DEFAULT;
	configuration.max_clients = SERVER_MAX_CLIENTS_DEFAULT;
}

