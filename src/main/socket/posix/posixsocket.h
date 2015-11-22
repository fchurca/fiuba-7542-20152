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
	struct sockaddr_in sockaddr;			// Dirección del socket.
public:
	PosixSocket();

	virtual ~PosixSocket();
public:
	bool Connect(std::string hostIp, size_t hostPort);
	bool Listen(size_t port, size_t maxConnections);
	std::shared_ptr<Socket> Accept();
	long Send(const void* data, size_t dataLength);
	long Recv(void* data, size_t dataLength);
	bool IsActive();
	void Activate();
	void deinit();

};
//-----------------------------------------------------------------------------
#endif /* SRC_MAIN_SOCKET_POSIX_POSIXSOCKET_H_ */
//-----------------------------------------------------------------------------
