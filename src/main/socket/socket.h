//-----------------------------------------------------------------------------
#ifndef SRC_MAIN_SOCKET_SOCKET_H_
#define SRC_MAIN_SOCKET_SOCKET_H_
//-----------------------------------------------------------------------------
class Socket {


public:
	void Connect(std::string hostIP, int hostPort) = 0;
	bool Listen(int maxConnections) = 0;
	bool Accept() = 0;
	int Send(const void* data, int dataLenght) = 0;
	int Recv(const void* data, int dataLenght) = 0;
	bool IsActive() = 0;
};


//-----------------------------------------------------------------------------
#endif /* SRC_MAIN_SOCKET_SOCKET_H_ */
//-----------------------------------------------------------------------------
