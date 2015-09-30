#include <sstream>
#include <algorithm>

#include "board.h"

using namespace std;

//-----------------------------------------------------------------------------
Board::Board(int sizeX, int sizeY, size_t dt) : sizeX(sizeX), sizeY(sizeY), dt(dt) {
	stringstream message;
	message << "Creating board " << this << " of size " << sizeX << "x" << sizeY;
	Logger::getInstance()->writeInformation(message.str());
	terrain.resize(sizeX * sizeY);
}

void Board::buildBoard() {
}

Entity & Board::getTerrain(size_t x, size_t y) {
	return *(terrain[(sizeX*y) + x]);
}

void Board::setTerrain(string name, size_t x, size_t y) {
	if (entityFactories.find(name) == entityFactories.end()) {
		Logger::getInstance()->writeError("No existe el tipo de entidad " + name);
	} else {
		terrain[(sizeX*y) + x] = entityFactories[name]->createEntity({(double)x, (double)y});
	}
}

shared_ptr<Entity> Board::findEntity(rectangle r) {
	auto it = find_if(entities.begin(), entities.end(), [r](shared_ptr<Entity> e) {
			return rectangle(e->getPosition(), e->size).intersects(r);
			});
	return (it == entities.end())? nullptr : *it;
}

shared_ptr<Entity> Board::createEntity(string name, r2 position) {
	if (entityFactories.find(name) == entityFactories.end()) {
		Logger::getInstance()->writeError("No existe el tipo de entidad " + name);
		return nullptr;
	}

	auto factory = entityFactories[name];
	if (findEntity(rectangle(position, factory->size))) {
		Logger::getInstance()->writeError("Lugar ya ocupado para entidad " + name);
		return nullptr;
	}

	auto pEntity = factory->createEntity(position);
	entities.push_back(pEntity);
	return pEntity;
}

shared_ptr<EntityFactory> Board::createEntityFactory(string name, r2 size, double speed) {
	auto pFactory = make_shared<EntityFactory>(name, size, speed, *this);
	entityFactories[name] = pFactory;
	return pFactory;
}

shared_ptr<Entity> Board::createProtagonist(string name, r2 position) {
	protagonist = createEntity(name, position);
	if (!protagonist) {
		Logger::getInstance()->writeError("Protagonista no creado " + name);
	}
	return protagonist;
}

Board::~Board() {
	stringstream message;
	message << "Killing Board " << this;
	Logger::getInstance()->writeInformation(message.str());
}

void Board::update() {
	for (size_t i =0; i < entities.size(); ++i){
		entities[i]->update();
	}
}

vector<shared_ptr<Entity>> Board::getEntities() {
	return entities;
}

Entity & Board::getProtagonist() {
	return * protagonist;
}

