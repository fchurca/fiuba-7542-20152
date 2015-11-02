#ifndef _REMOTE_CLIENT_H_
#define _REMOTE_CLIENT_H_

#include <memory>
#include <mutex>
#include <queue>
#include <string>
#include <thread>
#include <vector>

#include "../model/abstract_client.h"
#include "../model/entity.h"
#include "../model/geometry.h"

class Socket;

class RemoteClient : public AClient{
	protected:
		size_t frame;
		void setFrame();
		std::thread th;
		std::queue<size_t> deleted;
		std::mutex deletedMutex;
		bool running;
		std::shared_ptr<Socket> socket;
	public:
		RemoteClient(Game& owner, Player& player, std::shared_ptr<Socket> socket);
		~RemoteClient();
		void update();
		void run();
};

Socket& operator<<(Socket& socket, r2 r);
Socket& operator<<(Socket& socket, Entity& e);
Socket& operator<<(Socket& socket, Player& p);

#endif // _REMOTE_CLIENT_H_

