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
#ifdef _WIN32
	#include <winsock2.h>
	#include <ws2tcpip.h>

class WinSocket : Socket {

private:
	WSADATA wsaData;
	int sockfd;							// Filedescriptor del socket.
	struct addrinfo serv_addr;
	//analizar si es necesario
	struct sockaddr_in cli_addr;

private:
	bool status;
	unsigned int port;
	std::string ip;

public:
	WinSocket(unsigned int uiPort);
	virtual ~Socket();

public:
	void Connect(std::string hostIP, int hostPort);
	bool Listen(int maxConnections);
	Socket* Accept();
	int Send(const void* data, int dataLenght);
	int Recv(const void* data, int dataLenght);
	bool IsActive();

};
#endif
//-----------------------------------------------------------------------------
#endif /* SRC_SOCKET_WINSOCKET_H_ */
//-----------------------------------------------------------------------------
