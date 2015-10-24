#ifndef __CLIENTPARSER_H__
#define __CLIENTPARSER_H__

#include <iostream>
#include <fstream>
#include "yaml-cpp/yaml.h"
#include "generic_parser.h"
#include "../defines/defines.h"
#include "../log/logger.h"

using namespace std;

struct TagClientConfiguration {
	std::string server_ip;
	unsigned int server_port;
};


class ClientParser : GenericParser
{
private:
	void setClientConfiguration(const YAML::Node& node, TagClientConfiguration& configuration);
	void setClientConfigurationDefault(TagClientConfiguration& configuration);

public:
	ClientParser(std::string filename, std::string filenameDefault);
	TagClientConfiguration getClientConfiguracion();
	~ClientParser(void);
};

#endif