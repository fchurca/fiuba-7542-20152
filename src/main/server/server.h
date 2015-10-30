//-----------------------------------------------------------------------------
#ifndef MAIN_SERVER_SRC_SERVER_H_
#define MAIN_SERVER_SRC_SERVER_H_
//-----------------------------------------------------------------------------
#include <memory>
#include <thread>

#include "../model/game.h"
#include "../parser_yaml/server_parser.h"
#include "../socket/socket.h"
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
	Server(Game& game, ServerParser& parser);
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
