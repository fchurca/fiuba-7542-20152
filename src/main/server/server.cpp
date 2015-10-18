//-----------------------------------------------------------------------------
#include "server.h"
#include "../socket/posix/posixsocket.h"
//-----------------------------------------------------------------------------
Server::Server(Configuration config) {
	this->port = std::stoi(config.port);
	this->ip = config.ip;
	this->max_clients = config.max_clients;
	this->status = false;
	//this->socket = new PosixSocket(this->port);

}
//-----------------------------------------------------------------------------
Server::~Server() {
	// TODO Auto-generated destructor stub
}
//----------------------------------------------------------------------------
void call_from_thread(int age)
{
	std::cout << "feliz cumpelaños fiona " << age << std::endl;
}
//-----------------------------------------------------------------------------
bool Server::init()
{

	this->t = std::thread (call_from_thread, 26);

	return true;
}
//-----------------------------------------------------------------------------

