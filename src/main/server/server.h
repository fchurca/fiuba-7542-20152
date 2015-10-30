//-----------------------------------------------------------------------------
#ifndef MAIN_SERVER_SRC_SERVER_H_
#define MAIN_SERVER_SRC_SERVER_H_
//-----------------------------------------------------------------------------
#include <thread>
#include <memory>
#include "../socket/socket.h"
#include "../model/game.h"
//-----------------------------------------------------------------------------
class Server {

private:
	bool status;
	std::string address;
	int port;
	int max_clients;
	std::shared_ptr<Socket> socket;
	std::thread th;
	Game & game;

public:
	Server(Game& game);
	virtual ~Server();

public:
	bool init();
	void stop();
	void start();
	void run();

public:
	bool isActive();
};
//-----------------------------------------------------------------------------
#endif /* MAIN_SERVER_SRC_SERVER_H_ */
//-----------------------------------------------------------------------------
