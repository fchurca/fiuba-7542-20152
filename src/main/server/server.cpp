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
bool Server::isActive()
{
	return this->status;
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
		//ConexionCliente *conexionCLI = new ConexionCliente(socketCLI,
		//		this->admClientes);

		// Damos la orden de que comience a ejecutarse el hilo del cliente.
		conexionCLI->start();
		}

}
//-----------------------------------------------------------------------------
bool Server::init()
{
	this->socket =  new PosixSocket();
	this->socket->Listen(this->max_clients);

	//std::thread(start);
	// lo que sucede dentro de start es bloqueante x lo q se deberia ejectuar en un
	//thread aparte.

	this->start();

	return true;
}
//-----------------------------------------------------------------------------
void Server::stop()
{
	//Cambio el estado del server
	this->status = false;

	// Detenemos hilo hay q detener la funcion run
	// analizar si es suficiente con el cambio de estado en el server.


	// Cerramos el socket
	this->socket->deinit();

	// Mensaje de log


}
