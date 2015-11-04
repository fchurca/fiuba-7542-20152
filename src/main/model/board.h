#ifndef _MODEL_BOARD_H_
#define _MODEL_BOARD_H_
//-----------------------------------------------------------------------------
#include <string>
#include <vector>
#include <map>
#include <memory>
#include <mutex>
#include <queue>
//-----------------------------------------------------------------------------
#include "command.h"
#include "mixins.h"
#include "player.h"
#include "../log/logger.h"

class Entity;
class EntityFactory;
class ScenarioParser;
class RulesetParser;

class ABoard : public FrameMixin {
	protected:
		std::map<std::string, std::shared_ptr<Player>> players;
		std::map<std::string, std::shared_ptr<EntityFactory>> entityFactories;
		std::vector<std::shared_ptr<Entity>> entities;
		std::vector<std::shared_ptr<Entity>> terrain;
		std::queue<std::shared_ptr<Command>> commands;
		std::mutex commandMutex;
		enum BoardState {building, running, finished, error} state;
	public:
		long maxResources;
		int sizeX, sizeY;
		std::string name;
		size_t dt;

		ABoard(RulesetParser& rulesetParser, std::string name, int sizeX, int sizeY, long maxResources);
		virtual ~ABoard();

		enum ABoard::BoardState getState();
		bool isRunning();
		virtual void update();
		std::shared_ptr<Player> createPlayer(std::string name, bool human);
		Player& findPlayer(std::string name);
		virtual std::vector<std::shared_ptr<Player>> getPlayers();
		std::shared_ptr<EntityFactory> createEntityFactory(std::string name, r2 size, double speed, int sight_radius, bool solid, int capacity);
		std::shared_ptr<Entity> createEntity(std::string name, std::string playerName, r2 position);
		virtual std::shared_ptr<Entity> getTerrain(size_t x, size_t y);
		void setTerrain(std::string name, size_t x, size_t y);
		std::vector<std::shared_ptr<Entity>> getEntities();
		template<typename F> void mapEntities(F fun);
		template<typename Pred>
			std::vector<std::shared_ptr<Entity>> selectEntities(Pred pred);
		std::shared_ptr<Entity> findEntity(size_t id);
		std::shared_ptr<Entity> findEntity(rectangle r);
		std::shared_ptr<Entity> findEntity(r2 pos);

		void pushCommand(std::shared_ptr<Command> command);
	// Visited methods for dispatching execute(board, command)
		virtual void execute(StopCommand& command) = 0;
		virtual void execute(MoveCommand& command) = 0;
};

class SmartBoard : public ABoard {
	protected:
		SmartBoard();

	public:
		SmartBoard(RulesetParser& rulesetParser, ScenarioParser& scenarioParser);
		~SmartBoard();

		void update();
	// Visited methods for dispatching execute(board, command)
		void execute(StopCommand& command);
		void execute(MoveCommand& command);
};

#include "board.cxx"
//-----------------------------------------------------------------------------
#endif /* _MODEL_BOARD_H_ */
//-----------------------------------------------------------------------------
