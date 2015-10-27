//-----------------------------------------------------------------------------
#include "server.h"
#include "clientconexion.h"
#ifndef _WIN32
#include "../socket/posix/posixsocket.h"
#endif // ! _WIN32
#include "../model/game.h"

#include <iostream>

void idle_int(int param) {}

using namespace std;
//-----------------------------------------------------------------------------
Server::Server(Game& game) :
	port(8001),
	ip("127.0.0.1"),
	max_clients(4),
	status(false),
	socket(nullptr),
	game(game)
{
}

Server::Server(Configuration* config, Game& game) : game(game) {
	this->port = std::stoi(config->port);
	this->ip = config->ip;
	this->max_clients = config->max_clients;
	this->status = false;
	this->socket = 0;

}
//-----------------------------------------------------------------------------
Server::~Server() {
	stop();
}
//----------------------------------------------------------------------------
bool Server::isActive()
{
	return this->status && !game.willExit();
}
//-----------------------------------------------------------------------------
void Server::start() {
	th = thread(&Server::run, this);
}

void Server::run()
{
	cerr << "Server::run()" << endl;
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
	this->socket = nullptr;
#ifndef _WIN32
	this->socket =  new PosixSocket();
#endif // ! _WIN32

	if(this->socket->Listen(this->port,this->max_clients))
	{
		// si se establece el listen el server queda levantado
		this->status = true;
	}

	if (!status) {
		cerr << "Could not open a new socket at " << ip << ":" << port << endl;
		return false;
	}

	this->start();

	return true;
}
//-----------------------------------------------------------------------------
void Server::stop()
{
	//Cambio el estado del server
	if (status) {
		this->status = false;
		// Detenemos hilo hay q detener la funcion run
		th.join();
		// Cerramos el socket
		this->socket->deinit();
	}
	// TODO: Mensaje de log
}
//-----------------------------------------------------------------------------

