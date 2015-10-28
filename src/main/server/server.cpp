//-----------------------------------------------------------------------------
#include "server.h"
#include "clientconexion.h"
#include "../socket/socket.h"
#ifndef _WIN32
#include "../socket/posix/posixsocket.h"
#endif // ! _WIN32
#include "../model/game.h"
#include "../remote_client/remote_client.h"

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
		shared_ptr<Socket> socketCLI;

		// Aceptamos nuevo cliente accept es bloqueante
		//cuando se recibe un nuevo cliente, se genera una conexion para atenderlo y se lo carga en el vector
		//de conexiones

		// Fede, en este punto es donde si queres la conexion puede ser un "Cliente... y tener logica"
		//Osea cambiamos esa clase a lo que modela el cliente en el servidor y usa sockets para comunicacion
		socketCLI = this->socket->Accept();

		//Si hubo problemas en el socket salir y loggear
		if(!this->socket->IsActive() || !socketCLI) break;

		// Generamos una nueva conexión para ese cliente
		auto player = game.getAvailablePlayer();
		if (player) {
			game.addClient(make_shared<RemoteClient>(game, *player, socketCLI));
		}
	}

}
//-----------------------------------------------------------------------------
bool Server::init()
{
	this->socket = Socket::create();

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
		auto dummy = Socket::create();
		dummy->Connect("localhost", port); // TODO: check address
		// Detenemos hilo hay q detener la funcion run
		th.join();
		// Cerramos el socket
		this->socket->deinit();
	}
	// TODO: Mensaje de log
}
//-----------------------------------------------------------------------------

