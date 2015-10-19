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
