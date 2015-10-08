//-----------------------------------------------------------------------------
#ifndef MAIN_SERVER_SRC_SERVER_H_
#define MAIN_SERVER_SRC_SERVER_H_
//-----------------------------------------------------------------------------
#include <thread>
//-----------------------------------------------------------------------------
#include "configuration.h"
//-----------------------------------------------------------------------------
class Server {

private:
	std::thread t;
	bool status;
	std::string ip;
	int port;
	int max_clients;

public:
	Server(Configuration config);
	virtual ~Server();

public:
	void init();
	void stop();
	void run();

public:
	bool isActive();
};
//-----------------------------------------------------------------------------
#endif /* MAIN_SERVER_SRC_SERVER_H_ */
//-----------------------------------------------------------------------------
