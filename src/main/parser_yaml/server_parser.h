#ifndef __SERVERPARSER_H__
#define __SERVERPARSER_H__

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

class ServerParser : public GenericParser{
private:
	void setServerConfiguration(const YAML::Node& node, TagServerConfiguration& configuration);
	void setServerConfigurationDefault(TagServerConfiguration& configuration);
	void setServerPaths(const YAML::Node& node, TagServerPaths& paths);
	void setServerPathsDefault(TagServerPaths& paths);

public:
	ServerParser(std::string filename, std::string filenameDefault);
	TagServerConfiguration getServerConfiguration();
	TagServerPaths getServerPaths();
	~ServerParser(void);
};

#endif
