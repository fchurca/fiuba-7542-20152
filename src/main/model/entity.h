//-----------------------------------------------------------------------------
#ifndef _MODEL_ENTITY_H_
#define _MODEL_ENTITY_H_
//-----------------------------------------------------------------------------
#include <string>
#include <deque>

#include "../defines/defines.h"
#include "geometry.h"
#include "player.h"

//-----------------------------------------------------------------------------

class ABoard;
class ResourceEntity;

class Entity {
	protected:
		r2 position;	// Position (tile)
		double speed;	// Speed (tiles/s)
		std::deque<r2> waypoints;
		bool deletable;
		size_t id;
		bool solid;
		double orientation;
		size_t frame;
		bool adjustPosition();
		Entity();
		virtual void collide(Entity* other);
		virtual void collide(Entity& other);
		virtual void collide(ResourceEntity& other);
		bool canEnter(rectangle r);
		bool canEnter(r2 newPosition);
		void setFrame();
	public:
		Player& owner;
		r2 size;
		virtual void addTarget(r2 newTarget);
		virtual void unsetTarget();
		r2 target();
		bool targeted();
		const std::string name;
		ABoard& board;
		r2 center();
		int sight_radius;
		int capacity;
		r2 getPosition();
		void setPosition(r2 newPos);
		r2 trajectory();
		double sqDistance();
		double distance();
		double getOrientation();
		void setOrientation(double newOrientation);
		Directions getDirection();// TODO: Pertenece a vista
		void setDeletable();
		bool getDeletable();
		size_t getId();
		void setId(size_t newId);

		template<typename L> void mapVisible(L fun);

		Entity(std::string name, ABoard& board, Player& owner, r2 position, r2 size, double speed, int sight_radius, bool solid, int capacity);
		~Entity();

		void update();

		size_t getFrame();
		void setFrame(size_t newFrame);

		bool operator==(Entity& other);
		bool operator!=(Entity& other);
};

class ResourceEntity : public Entity {
	protected:
		void collide(Entity& other);
		void collide(ResourceEntity& other);
	public:
		ResourceEntity(std::string name, ABoard& board, Player& owner, r2 position, r2 size, double speed, int sight_radius, bool solid, int capacity);
};

#include "entity.cxx"
//-----------------------------------------------------------------------------
#endif /* _MODEL_ENTITY_H_ */
//-----------------------------------------------------------------------------
