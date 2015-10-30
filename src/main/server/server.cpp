//-----------------------------------------------------------------------------
#include "server.h"
#include "../socket/socket.h"
#include "../model/game.h"
#include "../remote_client/remote_client.h"

#include <iostream>

void idle_int(int param) {}

using namespace std;
//-----------------------------------------------------------------------------
Server::Server(Game& game) :
	port(8001),
	address("127.0.0.1"),
	max_clients(4),
	status(false),
	socket(nullptr),
	game(game)
{
}

Server::Server(Game& game, ServerParser& parser) :
	status(false),
	socket(nullptr),
	game(game),
	port(parser.getServerConfiguration().port),
	address(parser.getServerConfiguration().ip),
	max_clients(parser.getServerConfiguration().max_clients)
{
}
//-----------------------------------------------------------------------------
Server::~Server() {
	stop();
}
//----------------------------------------------------------------------------
bool Server::isActive() {
	return this->status && !game.willExit();
}
//-----------------------------------------------------------------------------
void Server::start() {
	th = thread(&Server::run, this);
}

void Server::run() {
	while(this->isActive()) {
		shared_ptr<Socket> socketCLI;

		// Aceptamos nuevo cliente accept es bloqueante
		//cuando se recibe un nuevo cliente, se genera una conexion para atenderlo y se lo carga en el vector
		//de conexiones

		socketCLI = this->socket->Accept();

		if(socketCLI) {
			// Generamos una nueva conexión para ese cliente
			auto player = game.getAvailablePlayer();
			if (player) {
				game.addClient(make_shared<RemoteClient>(game, *player, socketCLI));
			}
		} else {
			//Si hubo problemas en el socket salir y loggear
			stop();
		}
	}
}
//-----------------------------------------------------------------------------
bool Server::init() {
	this->socket = Socket::create();

	if(this->socket->Listen(this->port,this->max_clients)) {
		// si se establece el listen el server queda levantado
		this->status = true;
	}

	if (!status) {
		return false;
	}

	this->start();

	return true;
}
//-----------------------------------------------------------------------------
void Server::stop() {
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

