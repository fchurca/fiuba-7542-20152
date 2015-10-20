#ifndef _REMOTE_CLIENT_H_
#define _REMOTE_CLIENT_H_

#include <mutex>
#include <queue>
#include <thread>

#include "../model/abstract_client.h"
#include "../parser_yaml/parser_yaml.h"

class RemoteClient : public AClient{
	protected:
		size_t frame;
		void setFrame();
		thread th;
		std::queue<size_t> deleted;
		std::mutex deletedMutex;
	public:
		void update();
		RemoteClient(Game& owner, Player& player);
		~RemoteClient();
};

#endif // _REMOTE_CLIENT_H_

