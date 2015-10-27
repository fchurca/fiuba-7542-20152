#ifndef _MODEL_REMOTEBOARD_H_
#define _MODEL_REMOTEBOARD_H_

#include <memory>

#include "../model/board.h"
#include "../socket/socket.h"

class RemoteBoard : public ABoard {
	protected:
		std::shared_ptr<Socket> socket;
	public:
		RemoteBoard(RulesetParser& rulesetParser);
		~RemoteBoard();

		void update();
	// Visited methods for dispatching execute(board, command)
		void execute(StopCommand& command);
		void execute(MoveCommand& command);
};

#endif // _MODEL_BOARD_H_

