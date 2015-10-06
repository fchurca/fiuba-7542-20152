#include <cmath>

#include "server_parser_yaml.h"


ServerParserYAML::ServerParserYAML(std::string filename) {
	this->filename = filename;
}

void ServerParserYAML::parse() {
	//Logger::getInstance()->writeInformation("YAML-CPP:Inicia el parseo del archivo de configuracion.");
	std::ifstream fin(this->filename);
	if (!fin) {
		//Logger::getInstance()->writeError("YAML-CPP:El archivo de configuracion no existe o no pudo ser abierto.");
		//Logger::getInstance()->writeInformation("YAML-CPP: se toma un archivo de escenario default.");
		setArchivoDefault();
	}
	else if (fin.peek() == EOF) {
		//Logger::getInstance()->writeError("YAML-CPP:El archivo de configuracion se encuentra vacio.");
		//Logger::getInstance()->writeInformation("YAML-CPP: se toma un archivo de escenario default.");
		setArchivoDefault();
	}
	else {
		try {
			YAML::Parser parser(fin);
			parser.GetNextDocument(this->doc);
			if (this->doc.Type() != YAML::NodeType::Map) {
				//Logger::getInstance()->writeError("YAML-CPP:La estructura del archivo de configuracion no es correcta.");
				//Logger::getInstance()->writeInformation("YAML-CPP: se toma un archivo de escenario default.");
				setArchivoDefault();
			}
			if (fin.is_open())
				fin.close();
		}
		catch (YAML::ParserException &e) {
			if (fin.is_open())
				fin.close();
			//Logger::getInstance()->writeError("YAML-CPP:" + std::string(e.what()));
			//Logger::getInstance()->writeInformation("YAML-CPP: se toma un archivo de escenario default.");
			//setArchivoDefault();
		}
	}
}

void ServerParserYAML::setArchivoDefault() {
	// Este metodo no puede fallar, el archivo default no puede contener errores.
	this->filename = CONFIG_FILE_PATH_DEFAULT;
	std::ifstream fin(this->filename.c_str());
	YAML::Parser parser(fin);
	parser.GetNextDocument(this->doc);
	if (fin.is_open())
		fin.close();
}

TagServerConfiguration ServerParserYAML::getServerConfiguration() {
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

void ServerParserYAML::setServerConfiguration(const YAML::Node& node, TagServerConfiguration& configuration) {
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

void ServerParserYAML::setServerConfigurationDefault(TagServerConfiguration& configuration) {
	configuration.ip = SERVER_IP_DEFAULT;
	configuration.port = SERVER_PORT_DEFAULT;
	configuration.max_clients = SERVER_MAX_CLIENTS_DEFAULT;
}

TagServerPaths ServerParserYAML::getServerPaths() {
	//Logger::getInstance()->writeInformation("YAML-CPP:Se obtiene la informacion de configuracion.");
	TagServerPaths paths;
	if (this->doc.FindValue("paths")) {
		const YAML::Node& nodePaths = this->doc["paths"];
		if (nodePaths.Type() == YAML::NodeType::Map) {
			setServerPaths(nodePaths, paths);
		}
		else {
			//Logger::getInstance()->writeWarning("YAML-CPP:El tag de configuracion no es del tipo Map. Ubicar" + ubicarNodo(conf.GetMark()));
			setServerPathsDefault(paths);
		}
	}
	else {
		//Logger::getInstance()->writeWarning("YAML-CPP:El tag de configuracion no existe en el archivo.");
		setServerPathsDefault(paths);
	}
	return paths;
}

void ServerParserYAML::setServerPaths(const YAML::Node& node, TagServerPaths& paths) {
	if (node.Type() == YAML::NodeType::Map) {
		if (!obtenerValorScalarAlfaNumerico(node, "original_path", paths.original_path)) {
			//Logger::getInstance()->writeWarning("YAML-CPP: Se toma por default (velocidad personaje).");
			paths.original_path = CONFIG_FILE_PATH_DEFAULT;
		}
		if (!obtenerValorScalarAlfaNumerico(node, "default_path", paths.default_path)) {
			//Logger::getInstance()->writeWarning("YAML-CPP: Se toma por default (velocidad personaje).");
			paths.default_path = CONFIG_FILE_PATH_DEFAULT;
		}
	}
	else {
		//Logger::getInstance()->writeWarning("YAML-CPP:El contenido del tag de configuracion no es del tipo Map. Ubicar" + ubicarNodo(node.GetMark()));
		setServerPathsDefault(paths);
	}
}

void ServerParserYAML::setServerPathsDefault(TagServerPaths& paths) {
	paths.original_path = CONFIG_FILE_PATH_DEFAULT;
	paths.default_path = CONFIG_FILE_PATH_DEFAULT;
}
std::string ServerParserYAML::intToString(int i) {
	string resultado;
	ostringstream aux;
	aux << i;
	resultado = aux.str();
	return resultado;
}

std::string ServerParserYAML::ubicarNodo(const YAML::Mark marca_nodo) {
	std::string salida;
	salida = " Linea: " + intToString(marca_nodo.line + 1) + " Columna: " + intToString(marca_nodo.column + 1);
	return salida;
}

bool ServerParserYAML::obtenerValorScalarAlfaNumerico(const YAML::Node & node, std::string tag, std::string & salida) {
	if (node.FindValue(tag)) {
		const YAML::Node& nodo_tag = node[tag];
		if (nodo_tag.Type() == YAML::NodeType::Scalar) {
			nodo_tag >> salida;
			return true;
		}
		//else
			//Logger::getInstance()->writeWarning("YAML-CPP:El contenido ubicado en: " + tag + " no es del tipo Scalar. Ubicar" + ubicarNodo(nodo_tag.GetMark()));
	}
	//else
		//Logger::getInstance()->writeWarning("YAML-CPP:El: " + tag + " no existe en el nodo. Ubicar" + ubicarNodo(node.GetMark()));
	return false;
}

bool ServerParserYAML::esNumero(std::string numero) {
	int i = 0;
	int cantDec = 0;
	int largo = numero.length();
	if (numero.empty() || ((numero[0] != '-') && (!isdigit(numero[0])))) {
		return false;
	}
	if (numero[0] == '-') {
		if (largo < 2)
			return false;
		if (!isdigit(numero[1]))
			return false;
		i = 1;
	}
	for (; i < largo; i++) {
		if (!isdigit(numero[i]) && numero[i] != '.')
			return false;
		else
			if (numero[i] == '.')
				cantDec++;
	}
	if (cantDec > 1)
		return false;
	return true;
}

bool ServerParserYAML::obtenerValorScalarNumericoPositivo(const YAML::Node & node, std::string tag, unsigned int & salida) {
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
					//else
					//	Logger::getInstance()->writeWarning("YAML-CPP:El contenido ubicado en: " + tag + " es un numero decimal. Ubicar" + ubicarNodo(nodo_tag.GetMark()));
				}
				//else
				//	Logger::getInstance()->writeWarning("YAML-CPP:El contenido ubicado en: " + tag + " es un numero negativo. Ubicar" + ubicarNodo(nodo_tag.GetMark()));
			}
			//else
			//	Logger::getInstance()->writeWarning("YAML-CPP:El contenido ubicado en: " + tag + " no es un numero. Ubicar" + ubicarNodo(nodo_tag.GetMark()));
		}
		//else
		//	Logger::getInstance()->writeWarning("YAML-CPP:El contenido ubicado en: " + tag + " no es del tipo Scalar. Ubicar" + ubicarNodo(nodo_tag.GetMark()));
	}		
	//else
	//	Logger::getInstance()->writeWarning("YAML-CPP:El: " + tag + " no existe en el nodo. Ubicar" + ubicarNodo(node.GetMark()));
	return false;
}
