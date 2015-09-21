#include "board.h"
#include <sstream>

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
		Logger::getInstance()->writeWarning("No existe el tipo de entidad " + name);
	} else {
		terrain[(sizeX*y) + x] = entityFactories[name]->createEntity(x, y);
	}
}

shared_ptr<Entity> Board::createEntity(string name, double x, double y) {
	shared_ptr<Entity> pEntity = nullptr;
	if (entityFactories.find(name) == entityFactories.end()) {
		Logger::getInstance()->writeWarning("No existe el tipo de entidad " + name);
	} else {
		pEntity = entityFactories[name]->createEntity(x, y);
		entities.push_back(pEntity);
	}
	return pEntity;
}

shared_ptr<EntityFactory> Board::createEntityFactory(string name, double size_x, double size_y, double speed) {
	auto pFactory = make_shared<EntityFactory>(name, size_x, size_y, speed, *this);
	entityFactories[name] = pFactory;
	return pFactory;
}

shared_ptr<Entity> Board::createProtagonist(string name, double x, double y) {
	protagonist = createEntity(name, x, y);
	if (!protagonist) {
		Logger::getInstance()->writeWarning("Protagonista no creado " + name);
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

