//-----------------------------------------------------------------------------
#ifndef SRC_MAIN_SOCKET_POSIX_POSIXSOCKET_H_
#define SRC_MAIN_SOCKET_POSIX_POSIXSOCKET_H_
//-----------------------------------------------------------------------------
#include <memory>
#include <string>

#include "../socket.h"
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
	bool Connect(std::string hostIp, int hostPort);
	bool Listen(unsigned int port, int maxConnections);
	std::shared_ptr<Socket> Accept();
	ssize_t Send(const void* data, size_t dataLength);
	ssize_t Recv(void* data, size_t dataLength);
	bool IsActive();
	void Activate();
	void deinit();

};
//-----------------------------------------------------------------------------
#endif /* SRC_MAIN_SOCKET_POSIX_POSIXSOCKET_H_ */
//-----------------------------------------------------------------------------
