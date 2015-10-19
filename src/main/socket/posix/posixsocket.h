//-----------------------------------------------------------------------------
#ifndef SRC_MAIN_SOCKET_POSIX_POSIXSOCKET_H_
#define SRC_MAIN_SOCKET_POSIX_POSIXSOCKET_H_
//-----------------------------------------------------------------------------
#include <string>
#include "../socket.h"
//-----------------------------------------------------------------------------
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
//-----------------------------------------------------------------------------
class PosixSocket : public Socket {
private:
	int sockfd;
	struct sockaddr_in sockaddr;			// Dirección del socket.
	bool status;
	unsigned int port;

public:
	PosixSocket();

	virtual ~PosixSocket();
public:
	bool Connect(std::string hostIP, int hostPort);
	bool Listen(int maxConnections);
	Socket* Accept();
	int Send(const void* data, int dataLenght);
	int Recv(const void* data, int dataLenght);
	bool IsActive();
	void deinit();

};
//-----------------------------------------------------------------------------
#endif /* SRC_MAIN_SOCKET_POSIX_POSIXSOCKET_H_ */
//-----------------------------------------------------------------------------
