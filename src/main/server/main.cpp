/**
 * Servidor para mantener partida multijugador
 *
 */
//-----------------------------------------------------------------------------
#include "configuration.h"
#include "server_parser_yaml.h"
#include "defines.h"
#include "server.h"
//-----------------------------------------------------------------------------
int main(int argc, char* args[]) {


	ServerParserYAML parser(CONFIG_FILE_PATH);
	Configuration config(parser);

	Server server(config);

	return 0;
}
//-----------------------------------------------------------------------------
