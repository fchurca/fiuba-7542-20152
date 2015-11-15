#ifndef __CLIENTPARSER_H__
#define __CLIENTPARSER_H__

#include <iostream>
#include <fstream>
#include "yaml-cpp/yaml.h"
#include "generic_parser.h"
#include "../defines/defines.h"
#include "../log/logger.h"

struct TagClientConfiguration {
	std::string address;
	unsigned int port;
};


class ClientParser : public GenericParser {
private:
	void setClientConfiguration(const YAML::Node& node, TagClientConfiguration& configuration);
	void setClientConfigurationDefault(TagClientConfiguration& configuration);

public:
	ClientParser(std::string filename);
	TagClientConfiguration getClientConfiguracion();
	~ClientParser(void);
};

#endif
