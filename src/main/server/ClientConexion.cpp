//-----------------------------------------------------------------------------
#include "ClientConexion.h"
//-----------------------------------------------------------------------------
ClientConexion::ClientConexion(Socket* clientSocket) {

	this->socket = clientSocket;
}
//-----------------------------------------------------------------------------
ClientConexion::~ClientConexion() {
	// TODO Auto-generated destructor stub
}
//-----------------------------------------------------------------------------
void ClientConexion::start()
{
	while(this->socket->IsActive())
	{
		//protocolo para clientes

	}

}
//-----------------------------------------------------------------------------
void ClientConexion::stop()
{

}
//-----------------------------------------------------------------------------
