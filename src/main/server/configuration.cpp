//-----------------------------------------------------------------------------
#include "configuration.h"
//-----------------------------------------------------------------------------
Configuration::Configuration(ServerParserYAML* parser) {

	parser->parse();
	TagServerConfiguration tc = parser->getServerConfiguration();
	this->ip = tc.ip;
	this->port = tc.port;
	this->max_clients = tc.max_clients;

	TagServerPaths tp = parser->getServerPaths(); //TO-DO ver donde guardar esto.


}
//-----------------------------------------------------------------------------
Configuration::~Configuration() {
	// TODO Auto-generated destructor stub
}
//-----------------------------------------------------------------------------

