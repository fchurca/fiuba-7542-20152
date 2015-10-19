//-----------------------------------------------------------------------------
#include "server.h"
#include "../socket/posix/posixsocket.h"
//-----------------------------------------------------------------------------
Server::Server(Configuration* config) {
	this->port = std::stoi(config->port);
	this->ip = config->ip;
	this->max_clients = config->max_clients;
	this->status = false;
	this->socket = 0;

}
//-----------------------------------------------------------------------------
Server::~Server() {
	// TODO Auto-generated destructor stub
}
//----------------------------------------------------------------------------
void call_from_thread(int age)
{

}
//-----------------------------------------------------------------------------
void Server::start()
{
	while(this->isActive()) {
		Socket *socketCLI = 0;

		// Aceptamos nuevo cliente
		socketCLI = this->socket->Accept();

		//Si hubo problemas en el socket salir y loggear
		if(!this->socket->IsActive() || !socketCLI) break;

		// Generamos una nueva conexión para ese cliente
		ConexionCliente *conexionCLI = new ConexionCliente(socketCLI,
				this->admClientes, this->admCuentas, this->logger);

		// Damos la orden de que comience a ejecutarse el hilo del cliente.
		conexionCLI->start();
		}

}
//-----------------------------------------------------------------------------
bool Server::init()
{
	this->socket =  new PosixSocket();
	this->socket->Listen(this->max_clients);

	//std::thread(run);
	// lo que sucede dentro de run es bloqueante x lo q se deberia ejectuar en un
	//thread aparte.

	this->start();

	return true;
}
//-----------------------------------------------------------------------------
