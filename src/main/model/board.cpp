#include <sstream>
#include <algorithm>

#include "board.h"
#include "entity.h"
#include "entity_factory.h"

#include "../parser_yaml/parser_yaml.h"

using namespace std;

//-----------------------------------------------------------------------------
ABoard::ABoard(string name, size_t dt, int sizeX, int sizeY, long maxResources) :
	name(name),
	dt(dt),
	sizeX(sizeX), sizeY(sizeY),
	maxResources(maxResources),
	frame(0)
{}

size_t ABoard::getFrame() {
	return frame;
}

std::vector<std::shared_ptr<Player>> ABoard::getPlayers() {
	std::vector<std::shared_ptr<Player>> ret;
	for(auto& i : players) {
		ret.push_back(i.second);
	}
	return ret;
}

shared_ptr<Player> ABoard::createPlayer(string name, bool human) {
	if (players.find(name) != players.end()) {
		Logger::getInstance()->writeError("Jugador " + name + " ya existe");
		return nullptr;
	}
	return (players[name] = make_shared<Player>(*this, name, human));
}

shared_ptr<Entity> ABoard::createEntity(string name, string playerName, r2 position) {
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

shared_ptr<EntityFactory> ABoard::createEntityFactory(string name, r2 size, double speed, int sight_radius, bool solid, int capacity) {
	shared_ptr<EntityFactory> pFactory;
	if(name == "carne" || name == "oro") {
		pFactory = make_shared<ResourceEntityFactory>(name, size, speed, sight_radius, solid, capacity, *this);
	} else {
		pFactory = make_shared<EntityFactory>(name, size, speed, sight_radius, solid, capacity, *this);
	}
	entityFactories[name] = pFactory;
	return pFactory;
}

shared_ptr<Entity> ABoard::getTerrain(size_t x, size_t y) {
	return terrain[(sizeX*y) + x];
}

void ABoard::setTerrain(string name, size_t x, size_t y) {
	if (entityFactories.find(name) == entityFactories.end()) {
		Logger::getInstance()->writeError("No existe el tipo de entidad " + name);
	} else {
		terrain[(sizeX*y) + x] = entityFactories[name]->createEntity(*players[DEFAULT_PLAYER_NAME], {(double)x, (double)y});
	}
}

vector<shared_ptr<Entity>> ABoard::getEntities() {
	return entities;
}

shared_ptr<Entity> ABoard::findEntity(size_t id) {
	auto it = find_if(entities.begin(), entities.end(), [id](shared_ptr<Entity> e) {
			return e?e->getId() == id:false;
			});
	return (it == entities.end())? nullptr : *it;
}

shared_ptr<Entity> ABoard::findEntity(rectangle r) {
	auto it = find_if(entities.begin(), entities.end(), [r](shared_ptr<Entity> e) {
			return rectangle(e->getPosition(), e->size).intersects(r);
			});
	return (it == entities.end())? nullptr : *it;
}

shared_ptr<Entity> ABoard::findEntity(r2 pos) {
	return findEntity(rectangle(pos, {0,0}));
}

void ABoard::pushCommand(std::shared_ptr<Command> command) {
	commandMutex.lock();
	commands.push(command);
	commandMutex.unlock();
}

void ABoard::update() {
	frame++;
	for(size_t i = 0; i < entities.size();) {
		if (entities[i]->getDeletable()) {
			entities.erase(entities.begin() + i);
		} else {
			i++;
		}
	}
	commandMutex.lock();
	while(commands.size() > 0) {
		commands.front()->execute(*this);
		commands.pop();
	}
	commandMutex.unlock();
	for(auto& p : players) {
		p.second->update();
	}
}


Board::Board(ParserYAML& parser) :
	ABoard(parser.getEscenario().nombre,
			parser.getConfiguracion().dt,
			parser.getEscenario().size_x, parser.getEscenario().size_y,
			parser.getEscenario().max_resources)
{
	stringstream message;
	message << "Creating board " << this << " of size " << sizeX << "x" << sizeY;
	Logger::getInstance()->writeInformation(message.str());
	terrain.resize(sizeX * sizeY);

	createEntityFactory(PROTAGONISTA_DEFAULT_NOMBRE, {PROTAGONISTA_DEFAULT_ANCHO_BASE, PROTAGONISTA_DEFAULT_ALTO_BASE}, VELOCIDAD_PERSONAJE_DEFAULT, ENTIDAD_DEFAULT_SIGHT_RADIUS,true, ENTIDAD_DEFAULT_CAPACITY);
	createEntityFactory(ENTIDAD_DEFAULT_NOMBRE, {ENTIDAD_DEFAULT_ANCHO_BASE, ENTIDAD_DEFAULT_ALTO_BASE}, ENTIDAD_DEFAULT_SPEED, ENTIDAD_DEFAULT_SIGHT_RADIUS, true, ENTIDAD_DEFAULT_CAPACITY);
	createEntityFactory(TERRENO_DEFAULT_NOMBRE, {TERRENO_DEFAULT_ANCHO_BASE, TERRENO_DEFAULT_ALTO_BASE}, TERRENO_DEFAULT_SPEED, TERRENO_DEFAULT_SIGHT_RADIUS, false, TERRENO_DEFAULT_CAPACITY);
	createPlayer(DEFAULT_PLAYER_NAME, false);

	for(auto& t : parser.getTiposEntidades()) {
		createEntityFactory(t.nombre, {t.ancho_base, t.alto_base}, t.speed, t.sight_radius, t.solid, t.capacity);
	}
	for(auto& t : parser.getTiposTerrenos()) {
		createEntityFactory(t.nombre, {t.ancho_base, t.alto_base}, t.speed, t.sight_radius, t.solid, t.capacity); 
	}
	for (auto& t : parser.getTiposRecursos()) {
		createEntityFactory(t.nombre, { t.ancho_base, t.alto_base }, t.speed, t.sight_radius, t.solid, t.capacity);
	}
	auto te = parser.getEscenario();
	for(auto& t : te.terrenos) {
		setTerrain(t.tipoEntidad, t.pos_x, t.pos_y);
	}
	// Relleno con TERRENO_DEFAULT
	for(size_t x = 0; x < sizeX; x++) {
		for(size_t y = 0; y < sizeY; y++) {
			if (!getTerrain(x, y)) {
				setTerrain(TERRENO_DEFAULT_NOMBRE, x, y);
			}
		}
	}

	for (auto& jugador : te.jugadores) {
		createPlayer(jugador.name, jugador.isHuman);
		for (auto& entidadJugador : jugador.entidades) {
			if (!createEntity(entidadJugador.tipoEntidad,jugador.name , { (double)entidadJugador.pos_x, (double)entidadJugador.pos_y })) {
				Logger::getInstance()->writeInformation("Se crea un protagonista default");
				createEntity(PROTAGONISTA_DEFAULT_NOMBRE, jugador.name, { PROTAGONISTA_DEFAULT_POSX, PROTAGONISTA_DEFAULT_POSY });
			}
		}
	}

	for(auto& t : te.entidades) {
		createEntity(t.tipoEntidad, DEFAULT_PLAYER_NAME, {(double)t.pos_x,(double)t.pos_y});
	}

	// posinicialización
	for(auto& f : entityFactories) {
		f.second->populate();
	}
}

Board::~Board() {
	stringstream message;
	message << "Killing Board " << this;
	Logger::getInstance()->writeInformation(message.str());
}

void Board::update() {
	ABoard::update();
	for(auto& e : entities) {
		e->update();
	}
}

Player& Board::findPlayer(string name) {
	return *(players.find(name)->second);
}

void Board::execute(StopCommand& command) {
	auto e = findEntity(command.entityId);
	if (e) {
		e->unsetTarget();
	}
}

void Board::execute(MoveCommand& command) {
	auto e = findEntity(command.entityId);
	if (e) {
		e->addTarget(command.position);
	}
}

