#ifndef _REMOTE_CLIENT_H_
#define _REMOTE_CLIENT_H_

#include "../model/abstract_client.h"
#include "../parser_yaml/parser_yaml.h"

class RemoteClient : public AClient{
	protected:
		size_t frame;
		void setFrame();
	public:
		void update();
		RemoteClient(Game& owner, Player& player);
};

#endif // _REMOTE_CLIENT_H_

