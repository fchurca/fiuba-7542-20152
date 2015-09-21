#include "board.h"
#include <iostream>

using namespace std;

//-----------------------------------------------------------------------------
void Board::buildBoard() {
	terrain.resize(sizeX * sizeY);
}

Entity & Board::getTerrain(size_t x, size_t y) {
	return *(terrain[(sizeX*y) + x]);
}

void Board::setTerrain(string name, size_t x, size_t y) {
	if (entityFactories.find(name) == entityFactories.end()) 
		Logger::getInstance()->writeWarning("No existe el tipo de entidad " + name);
	else
		terrain[(sizeX*y) + x] = entityFactories[name]->createEntity(x, y);
}

std::shared_ptr<Entity> Board::createEntity(std::string name, double x, double y) {
	if (entityFactories.find(name) == entityFactories.end()) {
		Logger::getInstance()->writeWarning("No existe el tipo de entidad " + name);
	}
	auto pEntity = entityFactories[name]->createEntity(x, y);
	entities.push_back(pEntity);
	return pEntity;
}

std::shared_ptr<EntityFactory> Board::createEntityFactory(std::string name, double size_x, double size_y, double speed) {
	auto pFactory = std::make_shared<EntityFactory>(name, size_x, size_y, speed, *this);
	entityFactories[name] = pFactory;
	return pFactory;
}

void Board::createProtagonist(std::string name, double x, double y) {
	protagonist = createEntity(name, x, y);
}

Board::Board(int sizeX, int sizeY) : sizeX(sizeX), sizeY(sizeY) {
	cerr << "Creating board " << this << " of size " << sizeX << "x" << sizeY << endl;
	dt = 50; // TRAER DE CONFIGURACIÓN
}

Board::~Board() {
	cerr << "Killing Board " << this << endl;
}

void Board::update() {
	// cerr << "Board " << this << " updating" << endl;
	for (std::size_t i =0; i < entities.size(); ++i){
		entities[i]->update();
	}
	// cerr << "Board " << this << " updated" << endl;
}

std::vector<std::shared_ptr<Entity>> Board::getEntities() {
	return entities;
}

Entity & Board::getProtagonist() {
	return * protagonist;
}

