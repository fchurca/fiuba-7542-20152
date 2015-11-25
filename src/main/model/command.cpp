#include "board.h"

Command::Command(size_t entityId) : entityId(entityId){}

Command::~Command() {}


StopCommand::StopCommand(size_t entityId) : Command(entityId){}

void StopCommand::execute(ABoard& board) {
	board.execute(*this);
}


PositionalCommand::PositionalCommand(std::size_t entityId, r2 position) :
	Command(entityId),
	position(position)
{}

PositionalCommand::~PositionalCommand() {}


MoveCommand::MoveCommand(size_t entityId, r2 position) :
	PositionalCommand(entityId, position)
{}

void MoveCommand::execute(ABoard& board) {
	board.execute(*this);
}


InteractionCommand::InteractionCommand(size_t entityId, size_t targetId) :
	Command(entityId),
	targetId(targetId)
{}

InteractionCommand::~InteractionCommand() {}


RepairCommand::RepairCommand(size_t entityId, size_t targetId) :
	InteractionCommand(entityId, targetId)
{}

void RepairCommand::execute(ABoard& board) {
//	board.execute(*this); // TODO
}


GatherCommand::GatherCommand(size_t entityId, size_t targetId) :
	InteractionCommand(entityId, targetId)
{}

void GatherCommand::execute(ABoard& board) {
//	board.execute(*this); // TODO
}


AttackCommand::AttackCommand(size_t entityId, size_t targetId) :
	InteractionCommand(entityId, targetId)
{}

void AttackCommand::execute(ABoard& board) {
//	board.execute(*this); // TODO
}


EntityTypeCommand::EntityTypeCommand(size_t entityId, string entityType) :
	Command(entityId),
	entityType(entityType)
{}

EntityTypeCommand::~EntityTypeCommand() {}


BuildCommand::BuildCommand(size_t entityId, r2 position, string entityType) :
	PositionalCommand(entityId, position),
	EntityTypeCommand(entityId, entityType)
{}

void BuildCommand::execute(ABoard& board) {
//	board.execute(*this); // TODO
}


CreateCommand::CreateCommand(size_t entityId, string entityType) :
	EntityTypeCommand(entityId, entityType)
{}

void CreateCommand::execute(ABoard& board) {
//	board.execute(*this); // TODO
}

