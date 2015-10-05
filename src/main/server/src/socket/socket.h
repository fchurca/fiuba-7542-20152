//-----------------------------------------------------------------------------
#ifndef SRC_SOCKET_SOCKET_H_
#define SRC_SOCKET_SOCKET_H_
//-----------------------------------------------------------------------------
#include <string>
//-----------------------------------------------------------------------------
/*
 * Interfaz comun para utilzar sockets. *
 */
//-----------------------------------------------------------------------------
class Socket {

private:

	int sockfd;							// Filedescriptor del socket.
	//struct sockaddr_in miDir;			// Dirección del socket.
	//struct sockaddr_in destinoDir;		// Dirección del socket destino.
	bool status;

public:
	Socket();
	virtual ~Socket();
public:

	void create();
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
