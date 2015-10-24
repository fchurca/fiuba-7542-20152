#ifndef __SERVERPARSERYAML_H__
#define __SERVERPARSERYAML_H__

#include <iostream>
#include <fstream>
#include <string>
#include "yaml-cpp/yaml.h"
#include "generic_parser.h"


#include "../defines/defines.h"
#include "../log/logger.h"

using namespace std;

struct TagServerConfiguration {
	std::string ip;
	unsigned int port;
	unsigned int max_clients;
};

struct TagServerPaths {
	std::string original_path;
	std::string default_path;
};

class ServerParserYAML : GenericParserYAML{
private:
	void setServerConfiguration(const YAML::Node& node, TagServerConfiguration& configuration);
	void setServerConfigurationDefault(TagServerConfiguration& configuration);
	void setServerPaths(const YAML::Node& node, TagServerPaths& paths);
	void setServerPathsDefault(TagServerPaths& paths);
	void setArchivoDefault();


public:
	ServerParserYAML(std::string filename, std::string filenameDefault);
	TagServerConfiguration getServerConfiguration();
	TagServerPaths getServerPaths();
	~ServerParserYAML(void);
};

#endif
