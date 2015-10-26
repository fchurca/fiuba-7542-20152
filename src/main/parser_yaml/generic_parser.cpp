#include <cmath>

#include "generic_parser.h"

using namespace std;

GenericParser::GenericParser(std::string filename, std::string filenameDefault) {
	this->filename = filename;
	this->filenameDefault = filenameDefault;
}

GenericParser::~GenericParser(void) {

}

void GenericParser::parse() {
	Logger::getInstance()->writeInformation("YAML-CPP:Inicia el parseo del archivo de configuracion.");
	std::ifstream fin(this->filename);
	if (!fin) {
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
		try {
			YAML::Parser parser(fin);
			parser.GetNextDocument(this->doc);
			if (this->doc.Type() != YAML::NodeType::Map) {
				Logger::getInstance()->writeError("YAML-CPP:La estructura del archivo de configuracion no es correcta.");
				Logger::getInstance()->writeInformation("YAML-CPP: se toma un archivo de escenario default.");
				setArchivoDefault();
			}
			if (fin.is_open())
				fin.close();
		}
		catch (YAML::ParserException &e) {
			if (fin.is_open())
				fin.close();
			Logger::getInstance()->writeError("YAML-CPP:" + std::string(e.what()));
			Logger::getInstance()->writeInformation("YAML-CPP: se toma un archivo de escenario default.");
			setArchivoDefault();
		}
	}
}

void GenericParser::setArchivoDefault() {
	// Este metodo no puede fallar, el archivo default no puede contener errores.
	this->filename = filenameDefault;
	std::ifstream fin(this->filename.c_str());
	YAML::Parser parser(fin);
	parser.GetNextDocument(this->doc);
	if (fin.is_open())
		fin.close();
}


bool GenericParser::esNumero(std::string numero) {
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

bool GenericParser::obtenerValorScalarNumericoPositivo(const YAML::Node & node, std::string tag, unsigned int & salida) {
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
bool GenericParser::obtenerValorScalarNumericoPositivo(const YAML::Node & node, std::string tag, long & salida) {
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

bool GenericParser::obtenerValorScalarNumericoPositivo(const YAML::Node & node, std::string tag, double & salida) {
	std::string numero;
	double num;
	if (node.FindValue(tag)) {
		const YAML::Node& nodo_tag = node[tag];
		if (nodo_tag.Type() == YAML::NodeType::Scalar) {
			nodo_tag >> numero;
			if (esNumero(numero)) {
				nodo_tag >> num;
				if (num >= 0) {
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

bool GenericParser::obtenerValorScalarAlfaNumerico(const YAML::Node & node, std::string tag, std::string & salida) {
	if (node.FindValue(tag)) {
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



std::string GenericParser::ubicarNodo(const YAML::Mark marca_nodo) {
	std::string salida;
	salida = " Linea: " + intToString(marca_nodo.line + 1) + " Columna: " + intToString(marca_nodo.column + 1);
	return salida;
}

std::string GenericParser::intToString(int i) {
	string resultado;
	ostringstream aux;
	aux << i;
	resultado = aux.str();
	return resultado;
}
