#ifndef _MODEL_REMOTEBOARD_H_
#define _MODEL_REMOTEBOARD_H_

#include <memory>
#include <vector>

#include "../model/board.h"
#include "../parser_yaml/client_parser.h"
#include "../socket/socket.h"

class RemoteBoard : public ABoard {
	protected:
		std::shared_ptr<Socket> socket;
		void updateResources(std::string playerName);
		bool flushOut();
	public:
		RemoteBoard(Game& game, RulesetParser& rulesetParser, ClientParser& clientParser);
		~RemoteBoard();

		void update();
	// Visited methods for dispatching execute(board, command)
		void execute(StopCommand& command);
		void execute(MoveCommand& command);
		void execute(BuildCommand& command);
		void execute(CreateCommand& command);
		void execute(GatherCommand& command);
		void execute(AttackCommand& command);
		void execute(RepairCommand& command);
};

#endif // _MODEL_BOARD_H_

