//-----------------------------------------------------------------------------
#ifndef SRC_MAIN_SOCKET_POSIX_POSIXSOCKET_H_
#define SRC_MAIN_SOCKET_POSIX_POSIXSOCKET_H_
//-----------------------------------------------------------------------------
class PosixSocket: public Socket {
public:
	PosixSocket();
	virtual ~PosixSocket();
public:
	void Connect(std::string hostIP, int hostPort);
	bool Listen(int maxConnections);
	bool Accept();
	int Send(const void* data, int dataLenght);
	int Recv(const void* data, int dataLenght);
	bool IsActive();


};
//-----------------------------------------------------------------------------
#endif /* SRC_MAIN_SOCKET_POSIX_POSIXSOCKET_H_ */
//-----------------------------------------------------------------------------
