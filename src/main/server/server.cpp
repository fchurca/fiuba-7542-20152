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
void Server::run()
{
	std::cout << "server run" << std::endl;
}
//-----------------------------------------------------------------------------
bool Server::init()
{
	//this->socket =  new PosixSocket();
	//this->socket->Listen(this->max_clients);

	std::thread(run);

	return true;
}
//-----------------------------------------------------------------------------
