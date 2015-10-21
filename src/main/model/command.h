#ifndef _COMMAND_H_
#define _COMMAND_H_

#include "geometry.h"

class ABoard;
class Board;

class Command {
	public:
		const size_t entityId;
		Command(size_t entityId);
	// Visitor methods for dispatching execute(board, command)
		virtual void execute(ABoard& board) = 0;
};

class StopCommand : public Command {
	public:
		StopCommand(size_t entityId);
	// Visitor methods for dispatching execute(board, command)
		void execute(ABoard& board);
};

class MoveCommand : public Command {
	public:
		const r2 position;
		MoveCommand(size_t entityId, r2 position);
	// Visitor methods for dispatching execute(board, command)
		void execute(ABoard& board);
};

#endif // _COMMAND_H_

