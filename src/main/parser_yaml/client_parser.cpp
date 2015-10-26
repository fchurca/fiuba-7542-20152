#include "client_parser.h"

using namespace std;

ClientParser::ClientParser(std::string filename, std::string filenameDefault) : GenericParser(filename, filenameDefault) {
}

ClientParser::~ClientParser(void) {

}

TagClientConfiguration ClientParser::getClientConfiguracion() {
	Logger::getInstance()->writeInformation("YAML-CPP:Se obtiene la informacion de configuracion del cliente.");
	TagClientConfiguration configuration;
	if (this->doc.FindValue("configuration")) {
		const YAML::Node& conf = this->doc["configuration"];
		if (conf.Type() == YAML::NodeType::Map) {
			setClientConfiguration(conf, configuration);
		}
		else {
			Logger::getInstance()->writeWarning("YAML-CPP:El tag de configuracion no es del tipo Map. Ubicar" + ubicarNodo(conf.GetMark()));
			setClientConfigurationDefault(configuration);
		}
	}
	else {
		Logger::getInstance()->writeWarning("YAML-CPP:El tag de configuracion no existe en el archivo.");
		setClientConfigurationDefault(configuration);
	}
	return configuration;
}

void ClientParser::setClientConfiguration(const YAML::Node& node, TagClientConfiguration& configuration) {
	if (node.Type() == YAML::NodeType::Map) {
		if (!obtenerValorScalarAlfaNumerico(node, "server_ip", configuration.server_ip)) {
			Logger::getInstance()->writeWarning("YAML-CPP: Se toma por default (server_ip).");
			configuration.server_ip = CLIENT_SERVER_IP_DEFAULT;
		}
		if (!obtenerValorScalarNumericoPositivo(node, "server_port", configuration.server_port)) {
			Logger::getInstance()->writeWarning("YAML-CPP: Se toma por default (server_port).");
			configuration.server_port = CLIENT_SERVER_PORT_DEFAULT;
		}
	}
	else {
		Logger::getInstance()->writeWarning("YAML-CPP:El contenido del tag de configuracion no es del tipo Map. Ubicar" + ubicarNodo(node.GetMark()));
		setClientConfigurationDefault(configuration);
	}
}

void ClientParser::setClientConfigurationDefault(TagClientConfiguration& configuration) {
	configuration.server_ip = CLIENT_SERVER_IP_DEFAULT;
	configuration.server_port = CLIENT_SERVER_PORT_DEFAULT;
}
