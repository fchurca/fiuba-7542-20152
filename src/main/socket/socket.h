//-----------------------------------------------------------------------------
#ifndef SRC_MAIN_SOCKET_SOCKET_H_
#define SRC_MAIN_SOCKET_SOCKET_H_
//-----------------------------------------------------------------------------
#include <string>
#include <memory>
//-----------------------------------------------------------------------------
class Socket {
protected:
	int sockfd;
	bool status;
	int port;
	std::string ip;

public:
	Socket(){};
	virtual ~Socket() {};
public:
	virtual bool Connect(std::string hostIP, int hostPort) = 0;
	virtual bool Listen(unsigned int port, int maxConnections) = 0;
	virtual std::shared_ptr<Socket> Accept() = 0;
	virtual int Send(const void* data, int dataLenght) = 0;
	virtual int Recv(const void* data, int dataLenght) = 0;
	virtual bool IsActive() = 0;
	virtual void Activate() = 0;
	virtual void deinit() = 0;
};


//-----------------------------------------------------------------------------
#endif /* SRC_MAIN_SOCKET_SOCKET_H_ */
//-----------------------------------------------------------------------------
