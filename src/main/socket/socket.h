//-----------------------------------------------------------------------------
#ifndef SRC_MAIN_SOCKET_SOCKET_H_
#define SRC_MAIN_SOCKET_SOCKET_H_
//-----------------------------------------------------------------------------
#include <string>
#include <memory>
#include <vector>

#include "../model/entity.h"

#ifdef _WIN32
#include <winsock2.h>
#else // !_WIN32
#include <arpa/inet.h>
#endif //!_WIN32

//-----------------------------------------------------------------------------
class Socket : public EntityVisitor {
protected:
	size_t sockfd;
	bool status;
	size_t port;
	std::string ip;
	size_t inBufferIndex;

public:
	Socket();
	virtual ~Socket();

public:
	static std::shared_ptr<Socket> create();
	virtual bool Connect(std::string hostIP, size_t hostPort) = 0;
	virtual bool Listen(size_t port, size_t maxConnections) = 0;
	virtual std::shared_ptr<Socket> Accept() = 0;
	virtual long Send(const void* data, size_t dataLength) = 0;
	virtual long Recv(void* data, size_t dataLength) = 0;
	virtual bool IsActive() = 0;
	virtual void Activate() = 0;
	virtual void deinit() = 0;

public:
	std::vector<char> inBuffer;
	std::vector<char> outBuffer;

	Socket& operator<<(char c);
	Socket& operator<<(bool b);
	Socket& operator<<(int i);
	Socket& operator<<(long l);
	Socket& operator<<(size_t s);
	Socket& operator<<(double d);
	Socket& operator<<(std::string s);

	Socket& operator>>(char& c);
	Socket& operator>>(bool& b);
	Socket& operator>>(long& l);
	Socket& operator>>(size_t& s);
	Socket& operator>>(int& i);
	Socket& operator>>(double& d);
	Socket& operator>>(std::string& s);

	bool flushIn();
	bool oFlushIn();
	bool flushOut();

	Socket& operator<<(r2 r);
	Socket& operator<<(Entity& e);
	Socket& operator<<(Player& p);

	void visit(Entity& e);
	void visit(Building& e);
	void visit(UnfinishedBuilding& e);
	void visit(Resource& e);
	void visit(Unit& e);
	void visit(Flag& e);
};


//-----------------------------------------------------------------------------
#endif /* SRC_MAIN_SOCKET_SOCKET_H_ */
//-----------------------------------------------------------------------------
