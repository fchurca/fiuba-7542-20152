#include "board.h"
#include <iostream>

using namespace std;

//-----------------------------------------------------------------------------
void Board::buildBoard(ParserYAML* parser) {
	terrain.resize(sizeX * sizeY);

	// CARGO MAPA PARA PRUEBA
	createEntityFactory("agua", 1, 1, 0);
	createEntityFactory("pasto", 1, 1, 0);
	createEntityFactory("piedra", 1, 1, 0);
	createEntityFactory("chancho", 1, 1, 0);
	createEntityFactory("mago", 1, 1, 0);

	setTerrain("agua", 0, 0);
	setTerrain("agua", 0, 1);
	setTerrain("agua", 0, 2);
	setTerrain("pasto", 0, 3);
	setTerrain("pasto", 0, 4);

	setTerrain("pasto", 1, 0);
	setTerrain("pasto", 1, 1);
	setTerrain("pasto", 1, 2);
	setTerrain("pasto", 1, 3);
	setTerrain("pasto", 1, 4);

	setTerrain("pasto", 2, 0);
	setTerrain("piedra", 2, 1);
	setTerrain("piedra", 2, 2);
	setTerrain("piedra", 2, 3);
	setTerrain("piedra", 2, 4);

	setTerrain("pasto", 3, 0);
	setTerrain("piedra", 3, 1);
	setTerrain("piedra", 3, 2);
	setTerrain("piedra", 3, 3);
	setTerrain("pasto", 3, 4);

	createEntity("chancho", 1, 1);

	createProtagonist("mago", 2, 2);

	setTerrain("pasto", 4, 0);
	setTerrain("pasto", 4, 1);
	setTerrain("pasto", 4, 2);
	setTerrain("pasto", 4, 3);
	setTerrain("pasto", 4, 4);

	for(size_t x = 0; x < sizeX; x++) {
		for(size_t y = 0; y < sizeY; y++) {
			if (&getTerrain(x, y) == NULL) {
				setTerrain("pasto", x, y);
			}
		}
	}
}

Entity & Board::getTerrain(size_t x, size_t y) {
	return *(terrain[(sizeX*y) + x]);
}

void Board::setTerrain(string name, size_t x, size_t y) {
	terrain[(sizeX*y) + x] = entityFactories[name]->createEntity(x, y);
}

std::shared_ptr<Entity> Board::createEntity(std::string name, double x, double y) {
	if (entityFactories.find(name) == entityFactories.end()) {
		throw "Not found!"; // TODO: Moar elegance
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

