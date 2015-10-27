#ifndef _REMOTE_CLIENT_H_
#define _REMOTE_CLIENT_H_

#include <mutex>
#include <queue>
#include <thread>
#include <memory>

#include "../model/abstract_client.h"

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

#endif // _REMOTE_CLIENT_H_

