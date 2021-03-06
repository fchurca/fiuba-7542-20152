#ifndef __SERVERPARSER_H__
#define __SERVERPARSER_H__

#include <fstream>
#include <string>
#include "yaml-cpp/yaml.h"
#include "generic_parser.h"


#include "../defines/defines.h"
#include "../log/logger.h"

struct TagServerConfiguration {
	std::string ip;
	unsigned int port;
	unsigned int max_clients;
};

class ServerParser : public GenericParser{
private:
	void setServerConfiguration(const YAML::Node& node, TagServerConfiguration& configuration);
	void setServerConfigurationDefault(TagServerConfiguration& configuration);

public:
	ServerParser(std::string filename);
	TagServerConfiguration getServerConfiguration();
	~ServerParser(void);
};

#endif
