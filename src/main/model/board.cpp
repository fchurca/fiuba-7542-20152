#include <sstream>
#include <algorithm>

#include "board.h"
#include "entity.h"
#include "entity_factory.h"

#include "../parser_yaml/ruleset_parser.h"
#include "../parser_yaml/scenario_parser.h"

using namespace std;

//-----------------------------------------------------------------------------
ABoard::ABoard(RulesetParser& rulesetParser, string name, int sizeX, int sizeY, long maxResources) :
	dt(rulesetParser.getConfiguracion().dt),
	name(name),
	sizeX(sizeX), sizeY(sizeY),
	maxResources(maxResources),
	state(BoardState::building)
{
	stringstream message;
	message << "Creating board " << this << " of size " << sizeX << "x" << sizeY;
	Logger::getInstance()->writeInformation(message.str());
	terrain.resize(sizeX * sizeY);

	createEntityFactory(PROTAGONISTA_DEFAULT_NOMBRE, {PROTAGONISTA_DEFAULT_ANCHO_BASE, PROTAGONISTA_DEFAULT_ALTO_BASE}, VELOCIDAD_PERSONAJE_DEFAULT, ENTIDAD_DEFAULT_SIGHT_RADIUS,true, ENTIDAD_DEFAULT_CAPACITY);
	createEntityFactory(ENTIDAD_DEFAULT_NOMBRE, {ENTIDAD_DEFAULT_ANCHO_BASE, ENTIDAD_DEFAULT_ALTO_BASE}, ENTIDAD_DEFAULT_SPEED, ENTIDAD_DEFAULT_SIGHT_RADIUS, true, ENTIDAD_DEFAULT_CAPACITY);
	createEntityFactory(TERRENO_DEFAULT_NOMBRE, {TERRENO_DEFAULT_ANCHO_BASE, TERRENO_DEFAULT_ALTO_BASE}, TERRENO_DEFAULT_SPEED, TERRENO_DEFAULT_SIGHT_RADIUS, false, TERRENO_DEFAULT_CAPACITY);
	createPlayer(DEFAULT_PLAYER_NAME, false);

	for(auto& t : rulesetParser.getTiposEntidades()) {
		createEntityFactory(t.nombre, {t.ancho_base, t.alto_base}, t.speed, t.sight_radius, t.solid, t.capacity);
	}
	for(auto& t : rulesetParser.getTiposTerrenos()) {
		createEntityFactory(t.nombre, {t.ancho_base, t.alto_base}, t.speed, t.sight_radius, t.solid, t.capacity);
	}
	for (auto& t : rulesetParser.getTiposRecursos()) {
		createEntityFactory(t.nombre, { t.ancho_base, t.alto_base }, t.speed, t.sight_radius, t.solid, t.capacity);
	}
	state = BoardState::running;
}

ABoard::~ABoard() {}

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

Player& ABoard::findPlayer(string name) {
	return *(players.find(name)->second);
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
	if (x >= sizeX || y >= sizeY) {
		return nullptr;
	}
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

enum ABoard::BoardState ABoard::getState() {
	return state;
}

bool ABoard::isRunning() {
	return state == BoardState::running;
}

void ABoard::update() {
	if(!isRunning()) {
		return;
	}
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
}


SmartBoard::SmartBoard(RulesetParser& rulesetParser, ScenarioParser& scenarioParser) :
	ABoard(rulesetParser,
			scenarioParser.getEscenario().nombre,
			scenarioParser.getEscenario().size_x, scenarioParser.getEscenario().size_y,
			scenarioParser.getEscenario().max_resources)
{
	stringstream message;
	message << "Creating SmartBoard " << this;
	Logger::getInstance()->writeInformation(message.str());

	auto te = scenarioParser.getEscenario();
	for(auto& t : te.terrenos) {
		setTerrain(t.tipoEntidad, (size_t)t.pos.x, (size_t)t.pos.y);
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
			if (!createEntity(entidadJugador.tipoEntidad,jugador.name , entidadJugador.pos)) {
				Logger::getInstance()->writeInformation("Se crea un protagonista default");
				createEntity(PROTAGONISTA_DEFAULT_NOMBRE, jugador.name, { PROTAGONISTA_DEFAULT_POSX, PROTAGONISTA_DEFAULT_POSY });
			}
		}
	}

	for(auto& t : te.entidades) {
		createEntity(t.tipoEntidad, DEFAULT_PLAYER_NAME, t.pos);
	}

	// posinicialización
	for(auto& f : entityFactories) {
		f.second->populate();
	}
}

SmartBoard::~SmartBoard() {
	stringstream message;
	message << "Killing SmartBoard " << this;
	Logger::getInstance()->writeInformation(message.str());
}

void SmartBoard::update() {
	if(!isRunning()) {
		return;
	}
	ABoard::update();
	for(auto& p : players) {
		p.second->update();
	}
	for(auto& e : entities) {
		e->update();
	}
}

void SmartBoard::execute(StopCommand& command) {
	auto e = findEntity(command.entityId);
	if (e) {
		if (e->owner.getAlive()) {
			e->unsetTarget();
		}
	}
}

void SmartBoard::execute(MoveCommand& command) {
	auto e = findEntity(command.entityId);
	if (e) {
		if (e->owner.getAlive()) {
			e->addTarget(command.position);
		}
	}
}

