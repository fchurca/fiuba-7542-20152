#include "board.h"
#include <iostream>

using namespace std;

//-----------------------------------------------------------------------------
void Board::buildBoard(ParserYAML* parser) {

	// CARGO MAPA PARA PRUEBA
	createEntityFactory("agua", 1, 1, 0);
	createEntityFactory("pasto", 1, 1, 0);
	createEntityFactory("piedra", 1, 1, 0);
	createEntityFactory("chancho", 1, 1, 0);

	createEntity("agua", 0, 0);
	createEntity("agua", 0, 1);
	createEntity("agua", 0, 2);
	createEntity("pasto", 0, 3);
	createEntity("pasto", 0, 4);

	createEntity("pasto", 1, 0);
	createEntity("pasto", 1, 1);
	createEntity("pasto", 1, 2);
	createEntity("pasto", 1, 3);
	createEntity("pasto", 1, 4);

	createEntity("pasto", 2, 0);
	createEntity("piedra", 2, 1);
	createEntity("piedra", 2, 2);
	createEntity("piedra", 2, 3);
	createEntity("piedra", 2, 4);

	createEntity("pasto", 3, 0);
	createEntity("piedra", 3, 1);
	createEntity("piedra", 3, 2);
	createEntity("piedra", 3, 3);
	createEntity("pasto", 3, 4);

	createProtagonist("chancho", 2, 2);

	createEntity("pasto", 4, 0);
	createEntity("pasto", 4, 1);
	createEntity("pasto", 4, 2);
	createEntity("pasto", 4, 3);
	createEntity("pasto", 4, 4);

	protagonist->setTarget(4, 2);
}

std::shared_ptr<Entity> Board::createEntity(std::string name, double x, double y) {
	if (entityFactories.find(name) == entityFactories.end()) {
		throw "Not found!"; // TODO: Moar elegance
	}
	auto pEntity = entityFactories[name]->createEntity(x, y);
	entities.push_back(pEntity);
	return pEntity;
}

std::shared_ptr<EntityFactory> Board::createEntityFactory(std::string name, int size_x, int size_y, double speed) {
	auto pFactory = std::make_shared<EntityFactory>(name, size_x, size_y, speed, *this);
	entityFactories[name] = pFactory;
	return pFactory;
}

void Board::createProtagonist(std::string name, double x, double y) {
	protagonist = createEntity(name, x, y);
}

Board::Board(int sizeX, int sizeY) : sizeX(sizeX), sizeY(sizeY) {
	cerr << "Creating board " << this << " of size " << sizeX << "x" << sizeY << endl;
	dt = 20; // TRAER DE CONFIGURACIÓN
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

