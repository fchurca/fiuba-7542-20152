/**
 * Servidor para mantener partida multijugador
 *
 */
//-----------------------------------------------------------------------------
#include "server_parser_yaml.h"
#include "configuration.h"
#include "defines.h"
#include "server.h"
//-----------------------------------------------------------------------------
int main(int argc, char* args[]) {


	ServerParserYAML* parser = new ServerParserYAML (CONFIG_FILE_PATH);
	Configuration config(parser);

	Server server(config);

	return 0;
}
//-----------------------------------------------------------------------------
