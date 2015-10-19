//-----------------------------------------------------------------------------
#ifndef SRC_MAIN_SERVER_CLIENTCONEXION_H_
#define SRC_MAIN_SERVER_CLIENTCONEXION_H_
//-----------------------------------------------------------------------------
#include "../socket/socket.h"
//-----------------------------------------------------------------------------
class ClientConexion {
public:
	ClientConexion(Socket* socket);
	virtual ~ClientConexion();

public:
	void start();
	void stop();

private:
	Socket *socket;
};
//-----------------------------------------------------------------------------
#endif /* SRC_MAIN_SERVER_CLIENTCONEXION_H_ */
//-----------------------------------------------------------------------------
