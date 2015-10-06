//-----------------------------------------------------------------------------
#ifndef SRC_SOCKET_SOCKET_H_
#define SRC_SOCKET_SOCKET_H_
//-----------------------------------------------------------------------------
/*
 * Interfaz comun para utilzar sockets. *
 */
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
#include <string>
//-----------------------------------------------------------------------------
#ifdef _WIN32
	//#include <windows.h>
	#include <winsock2.h>
	#include <ws2tcpip.h>

#endif


class Socket {

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
	Socket(unsigned int uiPort);
	virtual ~Socket();

public:
	void Connect(std::string hostIP, int hostPort);
	bool Listen(int maxConnections);
	bool Accept();
	int Send(const void* data, int dataLenght);
	int Recv(const void* data, int dataLenght);
	bool IsActive();

};
//-----------------------------------------------------------------------------
#endif /* SRC_SOCKET_SOCKET_H_ */
//-----------------------------------------------------------------------------
