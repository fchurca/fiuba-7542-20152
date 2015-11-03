#include "board.h"

Command::Command(size_t entityId) : entityId(entityId){}

Command::~Command() {}

StopCommand::StopCommand(size_t entityId) : Command(entityId){}

void StopCommand::execute(ABoard& board) {
	board.execute(*this);
}

MoveCommand::MoveCommand(size_t entityId, r2 position) :
	Command(entityId),
	position(position)
{}

void MoveCommand::execute(ABoard& board) {
	board.execute(*this);
}

