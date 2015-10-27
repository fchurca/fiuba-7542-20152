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
		std::vector<char> outBuffer;
		RemoteClient& operator<<(char c);
		RemoteClient& operator<<(int i);
		RemoteClient& operator<<(long l);
		RemoteClient& operator<<(size_t s);
		RemoteClient& operator<<(double d);
		RemoteClient& operator<<(r2 r);
		RemoteClient& operator<<(Entity& e);
		RemoteClient& operator<<(std::string s);
		void send();
	public:
		RemoteClient(Game& owner, Player& player, std::shared_ptr<Socket> socket);
		~RemoteClient();
		void update();
		void run();
};

#endif // _REMOTE_CLIENT_H_

