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
#include <winsock2.h>
#include <ws2tcpip.h>
//-----------------------------------------------------------------------------

class WinSocket : Socket {

private:
	WSADATA wsaData;
	struct addrinfo serv_addr;
	//analizar si es necesario
	struct sockaddr_in cli_addr;

public:
	WinSocket(unsigned int uiPort);
	virtual ~WinSocket();

public:
	void Connect(std::string hostIP, int hostPort);
	bool Listen(int maxConnections);
	Socket* Accept();
	int Send(const void* data, int dataLenght);
	int Recv(const void* data, int dataLenght);
	bool IsActive();

};
//-----------------------------------------------------------------------------
#endif /* SRC_SOCKET_WINSOCKET_H_ */
//-----------------------------------------------------------------------------
