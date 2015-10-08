//-----------------------------------------------------------------------------
#ifndef SRC_CONFIGURATION_H_
#define SRC_CONFIGURATION_H_
//-----------------------------------------------------------------------------
#include "server_parser_yaml.h"
//-----------------------------------------------------------------------------
class Configuration {
public:
	Configuration(ServerParserYAML parser);
	virtual ~Configuration();
public:
	std::string ip;
	std::string port;
	std::string path;
	int max_clients;

};
//-----------------------------------------------------------------------------
#endif /* SRC_CONFIGURATION_H_ */
//-----------------------------------------------------------------------------
