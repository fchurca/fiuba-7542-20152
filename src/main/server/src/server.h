//-----------------------------------------------------------------------------
#ifndef MAIN_SERVER_SRC_SERVER_H_
#define MAIN_SERVER_SRC_SERVER_H_
//-----------------------------------------------------------------------------
#include <SDL2/SDL.h>
#include <thread>
//-----------------------------------------------------------------------------
#include "configuration.h"
//-----------------------------------------------------------------------------
class Server {

private:
	std::thread t;
	int port;
	bool status;

public:
	Server();
	virtual ~Server();

public:
	void init(Configuration config);
	void stop();
	void run();

public:
	bool isActive();
};
//-----------------------------------------------------------------------------
#endif /* MAIN_SERVER_SRC_SERVER_H_ */
//-----------------------------------------------------------------------------
