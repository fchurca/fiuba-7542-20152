#ifndef _MODEL_BOARD_H_
#define _MODEL_BOARD_H_
//-----------------------------------------------------------------------------
#include <string>
#include <vector>
#include <map>
#include <memory>
//-----------------------------------------------------------------------------
#include "player.h"
#include "../log/logger.h"

class ABoard {
	protected:
		std::vector<std::shared_ptr<Entity>> entities;
	public:
		const int
			sizeX, sizeY;
		const std::string name;
		size_t dt;

		ABoard(std::string name, size_t dt, int sizeX, int sizeY);

		virtual void update() = 0;
		virtual std::vector<std::shared_ptr<Player>> getPlayers() = 0;
		virtual std::shared_ptr<Entity> getTerrain(size_t x, size_t y) = 0;
		template<typename Pred>
			std::vector<std::shared_ptr<Entity>> selectEntities(Pred pred);
};

class Entity;
class EntityFactory;
class ParserYAML;

class Board : public ABoard {

protected:
	std::map<std::string, std::shared_ptr<Player>> players;
	std::map<std::string, std::shared_ptr<EntityFactory>> entityFactories;
	Board();
	std::vector<std::shared_ptr<Entity>> terrain;

public:
	const long maxResources;
	Board(ParserYAML& parser);
	~Board();

	void setTerrain(std::string name, size_t x, size_t y);
	std::shared_ptr<Entity> getTerrain(size_t x, size_t y);
	std::shared_ptr<Entity> findEntity(rectangle r);
	std::shared_ptr<Entity> createEntity(std::string name, std::string playerName, r2 position);
	std::shared_ptr<Player> createPlayer(std::string name, bool human);
	std::shared_ptr<EntityFactory> createEntityFactory(std::string name, r2 size, double speed, int sight_radius, bool solid, int capacity);
	void update();
	std::vector<std::shared_ptr<Entity>> getEntities();
	Player& findPlayer(std::string name);
	std::vector<std::shared_ptr<Player>> getPlayers();
};

#include "board.cxx"
//-----------------------------------------------------------------------------
#endif /* _MODEL_BOARD_H_ */
//-----------------------------------------------------------------------------
