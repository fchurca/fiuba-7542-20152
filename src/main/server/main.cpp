/**
 * Servidor para mantener partida multijugador
 *
 */
//-----------------------------------------------------------------------------
#include <SDL2/SDL.h>

#include "../server.h"
//-----------------------------------------------------------------------------
int main(int argc, char* args[]) {

	Server server;

	Configuration config; //TO-DO: parsea un archivo yaml con los siguientes datos
	// 1) puerto y cantidad de clientes maximos
	// 2) Path a un archivo config.yaml donde se guarda el escenario de la partida
	// al igual q en la version stand alone.
	server.init(config);



	return 0;
}
//-----------------------------------------------------------------------------
