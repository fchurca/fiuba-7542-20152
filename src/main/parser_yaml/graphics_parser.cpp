#include "graphics_parser.h"

GraphicsParserYAML::GraphicsParserYAML(std::string filename, std::string filenameDefault) : GenericParserYAML(filename,filenameDefault) {
}

GraphicsParserYAML::~GraphicsParserYAML(void) {

}

TagConfiguracion GraphicsParserYAML::getConfiguracion() {
	Logger::getInstance()->writeInformation("YAML-CPP:Se obtiene la informacion de configuracion.");
	TagConfiguracion configuracion;
	if (this->doc.FindValue("configuracion")) {
		const YAML::Node& conf = this->doc["configuracion"];
		if (conf.Type() == YAML::NodeType::Map) {
			setConfiguracion(conf, configuracion);
		}
		else {
			Logger::getInstance()->writeWarning("YAML-CPP:El tag de configuracion no es del tipo Map. Ubicar" + ubicarNodo(conf.GetMark()));
			setConfiguracionDefault(configuracion);
		}
	}
	else {
		Logger::getInstance()->writeWarning("YAML-CPP:El tag de configuracion no existe en el archivo.");
		setConfiguracionDefault(configuracion);
	}
	return configuracion;
}

TagPantalla GraphicsParserYAML::getPantalla() {
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

void GraphicsParserYAML::setConfiguracion(const YAML::Node& node, TagConfiguracion& configuracion) {
	if (node.Type() == YAML::NodeType::Map) {
		if (!obtenerValorScalarNumericoPositivo(node, "dt", configuracion.dt)) {
			Logger::getInstance()->writeWarning("YAML-CPP: Se toma por default (dt).");
			configuracion.dt = DT_DEFAULT;
		}
	}
	else {
		Logger::getInstance()->writeWarning("YAML-CPP:El contenido del tag de configuracion no es del tipo Map. Ubicar" + ubicarNodo(node.GetMark()));
		setConfiguracionDefault(configuracion);
	}
}

void GraphicsParserYAML::setConfiguracionDefault(TagConfiguracion& configuracion) {
	Logger::getInstance()->writeWarning("YAML-CPP:Se toma configuracion por default.");
	configuracion.dt = DT_DEFAULT;
}


void GraphicsParserYAML::setPantalla(const YAML::Node& node, TagPantalla& pantalla) {
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

void GraphicsParserYAML::setPantallaDefault(TagPantalla& pantalla) {
	Logger::getInstance()->writeInformation("YAML-CPP:Se toma pantalla por default.");
	pantalla.alto = ALTO_DEFAULT;
	pantalla.ancho = ANCHO_DEFAULT;
	pantalla.velocidad_scroll = VELOCIDAD_SCROLL_DEFAULT;
	pantalla.margen_scroll = MARGEN_SCROLL_DEFAULT;
}