/**
 * Servidor para mantener partida multijugador
 *
 */
//-----------------------------------------------------------------------------
#include "configuration.h"
#include "server_parser_yaml.h"
#include "server_defines.h"

#include "server.h"
//-----------------------------------------------------------------------------
int main(int argc, char* args[]) {

	Server server;
	ServerParserYAML parser(CONFIG_FILE_PATH);
	parser.parse();
	TagServerConfiguration tc = parser.getServerConfiguration();
	TagServerPaths tp = parser.getServerPaths();
	Configuration config; //TO-DO: parsea un archivo yaml con los siguientes datos
	// 1) puerto y cantidad de clientes maximos
	// 2) Path a un archivo config.yaml donde se guarda el escenario de la partida
	// al igual q en la version stand alone.
	server.init(config);



	return 0;
}
//-----------------------------------------------------------------------------
