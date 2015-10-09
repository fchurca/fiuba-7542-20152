#include <sstream>
#include <algorithm>

#include "board.h"
#include "entity.h"
#include "entity_factory.h"

#include "../parser_yaml/parser_yaml.h"

using namespace std;

//-----------------------------------------------------------------------------
Board::Board(int sizeX, int sizeY, size_t dt, ParserYAML& parser) :
	sizeX(sizeX), sizeY(sizeY),
	dt(dt),
	maxResources(999999) // TODO: Define + config
{
	stringstream message;
	message << "Creating board " << this << " of size " << sizeX << "x" << sizeY;
	Logger::getInstance()->writeInformation(message.str());
	terrain.resize(sizeX * sizeY);

	// TODO: Levantar jugadores/facciones
	createPlayer("Franceses");
	createEntityFactory(PROTAGONISTA_DEFAULT_NOMBRE, {PROTAGONISTA_DEFAULT_ANCHO_BASE, PROTAGONISTA_DEFAULT_ALTO_BASE}, VELOCIDAD_PERSONAJE_DEFAULT, ENTIDAD_DEFAULT_SIGHT_RADIUS);
	createEntityFactory(ENTIDAD_DEFAULT_NOMBRE, {ENTIDAD_DEFAULT_ANCHO_BASE, ENTIDAD_DEFAULT_ALTO_BASE}, 0, ENTIDAD_DEFAULT_SIGHT_RADIUS);
	createEntityFactory(TERRENO_DEFAULT_NOMBRE, {TERRENO_DEFAULT_ANCHO_BASE, TERRENO_DEFAULT_ALTO_BASE}, 0, 0);
	createPlayer(DEFAULT_PLAYER_NAME);

	auto tc = parser.getConfiguracion();
	for(auto& t : parser.getTiposEntidades()) {
		createEntityFactory(t.nombre, {t.ancho_base, t.alto_base}, tc.vel_personaje, t.sight_radius); // LA VELOCIDAD DEBERIA VENIR DE CADA ENTIDAD
	}
	for(auto& t : parser.getTiposTerrenos()) {
		createEntityFactory(t.nombre, {t.ancho_base, t.alto_base}, 0, 0); 
	}
	auto te = parser.getEscenario();
	for(auto& t : te.terrenos) {
		setTerrain(t.tipoEntidad, t.pos_x, t.pos_y);
	}
	// Relleno con TERRENO_DEFAULT
	for(size_t x = 0; x < sizeX; x++) {
		for(size_t y = 0; y < sizeY; y++) {
			if (!&getTerrain(x, y)) {
				setTerrain(TERRENO_DEFAULT_NOMBRE, x, y);
			}
		}
	}
	// TODO: Levantar jugadores/facciones
	if(!createEntity(te.protagonista.tipoEntidad, "Franceses", {(double)te.protagonista.pos_x, (double)te.protagonista.pos_y})){
		Logger::getInstance()->writeInformation("Se crea un protagonista default");
		createEntity(PROTAGONISTA_DEFAULT_NOMBRE, "Franceses", {PROTAGONISTA_DEFAULT_POSX, PROTAGONISTA_DEFAULT_POSY});
	}

	// TODO: Revisar, por ahora todo el resto va para Gaia
	for(auto& t : te.entidades) {
		createEntity(t.tipoEntidad, DEFAULT_PLAYER_NAME, {(double)t.pos_x,(double)t.pos_y});
	}
}

Entity & Board::getTerrain(size_t x, size_t y) {
	return *(terrain[(sizeX*y) + x]);
}

void Board::setTerrain(string name, size_t x, size_t y) {
	if (entityFactories.find(name) == entityFactories.end()) {
		Logger::getInstance()->writeError("No existe el tipo de entidad " + name);
	} else {
		terrain[(sizeX*y) + x] = entityFactories[name]->createEntity(*players[DEFAULT_PLAYER_NAME], {(double)x, (double)y});
	}
}

shared_ptr<Entity> Board::findEntity(rectangle r) {
	auto it = find_if(entities.begin(), entities.end(), [r](shared_ptr<Entity> e) {
			return rectangle(e->getPosition(), e->size).intersects(r);
			});
	return (it == entities.end())? nullptr : *it;
}

shared_ptr<Entity> Board::createEntity(string name, string playerName, r2 position) {
	if (entityFactories.find(name) == entityFactories.end()) {
		Logger::getInstance()->writeError("No existe el tipo de entidad " + name);
		return nullptr;
	}

	auto factory = entityFactories[name];
	if (findEntity(rectangle(position, factory->size))) {
		Logger::getInstance()->writeError("Lugar ya ocupado para entidad " + name);
		return nullptr;
	}

	auto pEntity = factory->createEntity(*players[playerName], position);
	entities.push_back(pEntity);
	return pEntity;
}

shared_ptr<Player> Board::createPlayer(string name) {
	if (players.find(name) != players.end()) {
		Logger::getInstance()->writeError("Jugador " + name + " ya existe");
		return nullptr;
	}
	return (players[name] = make_shared<Player>(*this, name));
}

shared_ptr<EntityFactory> Board::createEntityFactory(string name, r2 size, double speed, int sight_radius) {
	auto pFactory = make_shared<EntityFactory>(name, size, speed, sight_radius, *this);
	entityFactories[name] = pFactory;
	return pFactory;
}

Board::~Board() {
	stringstream message;
	message << "Killing Board " << this;
	Logger::getInstance()->writeInformation(message.str());
}

void Board::update() {
	for(size_t i = 0; i < entities.size();) {
		if (entities[i]->getDeletable()) {
			entities.erase(entities.begin() + i);
		} else {
			i++;
		}
	}
	for(auto& e : entities) {
		e->update();
	}
	for(auto& p : players) {
		p.second->update();
	}
}

vector<shared_ptr<Entity>> Board::getEntities() {
	return entities;
}

Player& Board::findPlayer(string name) {
	return *(players.find(name)->second);
}
