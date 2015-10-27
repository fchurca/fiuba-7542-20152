#include "graphics_parser.h"

using namespace std;

GraphicsParser::GraphicsParser(std::string filename, std::string filenameDefault) : GenericParser(filename,filenameDefault) {
}

GraphicsParser::~GraphicsParser(void) {

}

TagPantalla GraphicsParser::getPantalla() {
	Logger::getInstance()->writeInformation("YAML-CPP:Se obtiene la informacion de la pantalla.");
	TagPantalla pantalla;
	if (this->doc.FindValue("pantalla")) {
		const YAML::Node& pant = this->doc["pantalla"];
		if (pant.Type() == YAML::NodeType::Map) {
			setPantalla(pant, pantalla);
		}
		else {
			Logger::getInstance()->writeWarning("YAML-CPP:El tag de pantalla no es del tipo Map. Ubicar" + ubicarNodo(pant.GetMark()));
			setPantallaDefault(pantalla);
		}
	}
	else {
		Logger::getInstance()->writeWarning("YAML-CPP:El tag de pantalla no existe en el archivo.");
		setPantallaDefault(pantalla);
	}
	return pantalla;
}


void GraphicsParser::setPantalla(const YAML::Node& node, TagPantalla& pantalla) {
	if (node.Type() == YAML::NodeType::Map) {
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
	else {
		Logger::getInstance()->writeWarning("YAML-CPP:El contenido del tag de pantalla no es del tipo Map. Ubicar" + ubicarNodo(node.GetMark()));
		setPantallaDefault(pantalla);
	}
}

void GraphicsParser::setPantallaDefault(TagPantalla& pantalla) {
	Logger::getInstance()->writeInformation("YAML-CPP:Se toma pantalla por default.");
	pantalla.alto = ALTO_DEFAULT;
	pantalla.ancho = ANCHO_DEFAULT;
	pantalla.velocidad_scroll = VELOCIDAD_SCROLL_DEFAULT;
	pantalla.margen_scroll = MARGEN_SCROLL_DEFAULT;
}