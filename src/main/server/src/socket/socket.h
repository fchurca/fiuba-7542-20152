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
	#include <windows.h>
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
	bool status;

public:
	Socket(unsigned int port);
	virtual ~Socket();

public:
	void connect(std::string hostIP, int hostPort);
	void listen(int maxConnections, int port, std::string ip);
	Socket* accept();
	int send(const void* data, int dataLenght);
	int recv(const void* data, int dataLenght);
	int close();
	bool isActive();

};
//-----------------------------------------------------------------------------
#endif /* SRC_SOCKET_SOCKET_H_ */
//-----------------------------------------------------------------------------
