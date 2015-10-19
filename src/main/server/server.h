//-----------------------------------------------------------------------------
#ifndef MAIN_SERVER_SRC_SERVER_H_
#define MAIN_SERVER_SRC_SERVER_H_
//-----------------------------------------------------------------------------
#include <thread>
#include "../socket/socket.h"
//-----------------------------------------------------------------------------
#include "configuration.h"
//-----------------------------------------------------------------------------
class Server {

private:
	bool status;
	std::string ip;
	int port;
	int max_clients;
	Socket* socket;

public:
	Server(Configuration* config);
	virtual ~Server();

public:
	bool init();
	void stop();
	void start();

public:
	bool isActive();
};
//-----------------------------------------------------------------------------
#endif /* MAIN_SERVER_SRC_SERVER_H_ */
//-----------------------------------------------------------------------------
