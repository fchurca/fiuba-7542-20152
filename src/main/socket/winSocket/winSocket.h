//-----------------------------------------------------------------------------
#ifndef SRC_SOCKET_WINSOCKET_H_
#define SRC_SOCKET_WINSOCKET_H_
//-----------------------------------------------------------------------------
/*
* Interfaz comun para utilzar sockets. *
*/
//-----------------------------------------------------------------------------
#include <string>
#include "../socket.h"
//-----------------------------------------------------------------------------
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
//-----------------------------------------------------------------------------

class WinSocket : public Socket {

private:
	SOCKET sockfd;
	struct sockaddr_in sockaddr;			// Dirección del socket.
	bool status;
	unsigned int port;

public:
	WinSocket();

	virtual ~WinSocket();
public:
	bool Connect(std::string hostIp, int hostPort);
	bool Listen(unsigned int port, int maxConnections);
	std::shared_ptr<Socket> Accept();
	int Send(const void* data, int dataLenght);
	int Recv(void* data, int dataLenght);
	bool IsActive();
	void Activate();
	void deinit();

};
//-----------------------------------------------------------------------------
#endif /* SRC_SOCKET_WINSOCKET_H_ */
//-----------------------------------------------------------------------------