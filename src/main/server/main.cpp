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
const std::string CMD_SALIR = "q";
//-----------------------------------------------------------------------------
int main(int argc, char* args[]) {


	ServerParserYAML* parser = new ServerParserYAML (CONFIG_FILE_PATH);
	Configuration* config = new Configuration(parser);

	Server server(config);

	if(!server.init())
	{
		//Pasar al logger si es necesario.
		std::cerr << "ERROR: No ha sido posible iniciar el servidor."
					<< std::endl;
	}

	std::string comando;

	// Esperamos a que se indique la finalización de la ejecución
	while(comando != CMD_SALIR)
		getline(std::cin, comando);

	delete(config);
	return 0;
}
//-----------------------------------------------------------------------------
