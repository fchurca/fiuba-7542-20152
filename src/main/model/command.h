#ifndef _COMMAND_H_
#define _COMMAND_H_

#include "geometry.h"

class ABoard;

class Command {
	public:
		const std::size_t entityId;
		Command(std::size_t entityId);
		virtual ~Command();
	// Visitor methods for dispatching execute(board, command)
		virtual void execute(ABoard& board) = 0;
};

class StopCommand : public Command {
	public:
		StopCommand(std::size_t entityId);
	// Visitor methods for dispatching execute(board, command)
		void execute(ABoard& board);
};

class PositionalCommand {
	public:
		PositionalCommand(r2 position);
		const r2 position;
		virtual ~PositionalCommand();
};

class MoveCommand : public Command, public PositionalCommand {
	public:
		MoveCommand(std::size_t entityId, r2 position);
	// Visitor methods for dispatching execute(board, command)
		void execute(ABoard& board);
};

class InteractionCommand {
	public:
		const std::size_t targetId;
		InteractionCommand(std::size_t targetId);
		virtual ~InteractionCommand();
};

class RepairCommand : public Command, public InteractionCommand {
	public:
		RepairCommand(std::size_t entityId, std::size_t targetId);
	// Visitor methods for dispatching execute(board, command)
		void execute(ABoard& board);
};

class AttackCommand : public Command, public InteractionCommand {
	public:
		AttackCommand(std::size_t entityId, std::size_t targetId);
	// TODO: Dispatching react()
	// Visitor methods for dispatching execute(board, command)
		void execute(ABoard& board);
};

class GatherCommand : public Command, public InteractionCommand {
	public:
		GatherCommand(std::size_t entityId, std::size_t targetId);
	// Visitor methods for dispatching execute(board, command)
		void execute(ABoard& board);
};

class EntityTypeCommand {
	public:
		const std::string entityType;
		EntityTypeCommand(std::string entityType);
		virtual ~EntityTypeCommand();
};

class BuildCommand : public Command, public PositionalCommand, public EntityTypeCommand {
	public:
		BuildCommand(std::size_t entityId, r2 position, std::string entityType);
	// Visitor methods for dispatching execute(board, command)
		void execute(ABoard& board);
};

class CreateCommand : public Command, public EntityTypeCommand {
	public:
		CreateCommand(std::size_t entityId, std::string entityType);
	// Visitor methods for dispatching execute(board, command)
		void execute(ABoard& board);
};

#endif // _COMMAND_H_

