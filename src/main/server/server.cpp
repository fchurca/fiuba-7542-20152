//-----------------------------------------------------------------------------
#include "server.h"
#include "../socket/posix/posixsocket.h"
#include "clientconexion.h"
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
	//while (this->)
}
//----------------------------------------------------------------------------
bool Server::isActive()
{
	return this->status;
}
//-----------------------------------------------------------------------------
void Server::start()
{
	while(this->isActive())
	{
		Socket *socketCLI = 0;

		// Aceptamos nuevo cliente accept es bloqueante
		//cuando se recibe un nuevo cliente, se genera una conexion para atenderlo y se lo carga en el vector
		//de conexiones

		// Fede, en este punto es donde si queres la conexion puede ser un "Cliente... y tener logica"
		//Osea cambiamos esa clase a lo que modela el cliente en el servidor y usa sockets para comunicacion
		socketCLI = this->socket->Accept();

		//Si hubo problemas en el socket salir y loggear
		if(!this->socket->IsActive() || !socketCLI) break;

		// Generamos una nueva conexión para ese cliente
		ClientConexion *conexion = new ClientConexion(socketCLI);

		//el admin podria ser un singleton (depende de como lo usemos luego en "la logica")
		this->adminClients.push_back(conexion);

		// Comienza a ejecutarse el hilo del cliente.
		conexion->start();
	}

}
//-----------------------------------------------------------------------------
bool Server::init()
{
	this->socket =  new PosixSocket();

	if(this->socket->Listen(this->port,this->max_clients))
	{
		// si se establece el listen el server queda levantado
		this->status = true;
	}

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
//-----------------------------------------------------------------------------

