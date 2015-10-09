//-----------------------------------------------------------------------------
#ifndef SRC_MAIN_SOCKET_SOCKET_H_
#define SRC_MAIN_SOCKET_SOCKET_H_
//-----------------------------------------------------------------------------
#include <string>
//-----------------------------------------------------------------------------
class Socket {
private:
	int sockfd;
	struct sockaddr_in sockaddr;			// Dirección del socket.
	bool status;
	int port;

public:
	Socket(unsigned int uiPort);
	virtual ~Socket();
public:
	virtual void Connect(std::string hostIP, int hostPort) = 0;
	virtual bool Listen(int maxConnections) = 0;
	virtual Socket* Accept() = 0;
	virtual int Send(const void* data, int dataLenght) = 0;
	virtual int Recv(const void* data, int dataLenght) = 0;
	virtual bool IsActive() = 0;
};


//-----------------------------------------------------------------------------
#endif /* SRC_MAIN_SOCKET_SOCKET_H_ */
//-----------------------------------------------------------------------------
