//-----------------------------------------------------------------------------
#ifndef MAIN_SERVER_SRC_SERVER_H_
#define MAIN_SERVER_SRC_SERVER_H_
//-----------------------------------------------------------------------------
#include <thread>
#include <memory>
#include "../socket/socket.h"
#include "../model/game.h"
//-----------------------------------------------------------------------------
#include "configuration.h"
#include "clientconexion.h"
//-----------------------------------------------------------------------------
class Server {

private:
	bool status;
	std::string ip;
	int port;
	int max_clients;
	std::shared_ptr<Socket> socket;
	std::thread th;
	Game & game;

public:
	Server(Game& game);
	Server(Configuration* config, Game& game);
	virtual ~Server();

public:
	bool init();
	void stop();
	void start();
	void run();

private:
	std::vector<ClientConexion*> adminClients;

public:
	bool isActive();
};
//-----------------------------------------------------------------------------
#endif /* MAIN_SERVER_SRC_SERVER_H_ */
//-----------------------------------------------------------------------------
