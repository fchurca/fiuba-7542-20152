#include <sstream>
#include <algorithm>

#include "board.h"
#include "entity.h"
#include "entity_factory.h"
#include "game.h"

#include "../parser_yaml/ruleset_parser.h"
#include "../parser_yaml/scenario_parser.h"

using namespace std;

//-----------------------------------------------------------------------------
ABoard::ABoard(Game& game, RulesetParser& rulesetParser, string name, int sizeX, int sizeY, long maxResources) :
	game(game),
	dt(rulesetParser.getConfiguracion().dt),
	name(name),
	sizeX(sizeX), sizeY(sizeY),
	maxResources(maxResources),
	state(BoardState::building),
	started(false)
{
	stringstream message;
	message << "Creating board " << this << " of size " << sizeX << "x" << sizeY;
	Logger::getInstance()->writeInformation(message.str());
	terrain.resize(sizeX * sizeY);

	createEntityFactory(PROTAGONISTA_DEFAULT_NOMBRE, {PROTAGONISTA_DEFAULT_ANCHO_BASE, PROTAGONISTA_DEFAULT_ALTO_BASE}, VELOCIDAD_PERSONAJE_DEFAULT, ENTIDAD_DEFAULT_SIGHT_RADIUS,true, ENTIDAD_DEFAULT_CAPACITY, ENTIDAD_DEFAULT_BEHAVIOUR);
	createEntityFactory(ENTIDAD_DEFAULT_NOMBRE, {ENTIDAD_DEFAULT_ANCHO_BASE, ENTIDAD_DEFAULT_ALTO_BASE}, ENTIDAD_DEFAULT_SPEED, ENTIDAD_DEFAULT_SIGHT_RADIUS, true, ENTIDAD_DEFAULT_CAPACITY, ENTIDAD_DEFAULT_BEHAVIOUR);
	createEntityFactory(TERRENO_DEFAULT_NOMBRE, {TERRENO_DEFAULT_ANCHO_BASE, TERRENO_DEFAULT_ALTO_BASE}, TERRENO_DEFAULT_SPEED, TERRENO_DEFAULT_SIGHT_RADIUS, false, TERRENO_DEFAULT_CAPACITY, TERRENO_DEFAULT_BEHAVIOUR);
	createPlayer(DEFAULT_PLAYER_NAME, false);

	for(auto& t : rulesetParser.getTiposUnidades()) {
		createEntityFactory(t.nombre, {t.ancho_base, t.alto_base}, t.speed, t.sight_radius, t.solid, t.capacity, t.behaviour);
	}
	for (auto& t : rulesetParser.getTiposEstructuras()) {
		createEntityFactory(t.nombre, { t.ancho_base, t.alto_base }, t.speed, t.sight_radius, t.solid, t.capacity, t.behaviour);
	}
	for(auto& t : rulesetParser.getTiposTerrenos()) {
		createEntityFactory(t.nombre, {t.ancho_base, t.alto_base}, t.speed, t.sight_radius, t.solid, t.capacity, t.behaviour);
	}
	for (auto& t : rulesetParser.getTiposRecursos()) {
		createEntityFactory(t.nombre, { t.ancho_base, t.alto_base }, t.speed, t.sight_radius, t.solid, t.capacity, t.behaviour);
	}
	state = BoardState::running;
}

ABoard::~ABoard() {
	if(started) {
		th.join();
	}
}

void ABoard::fillTerrain() {
	for(size_t x = 0; x < sizeX; x++) {
		for(size_t y = 0; y < sizeY; y++) {
			if (!getTerrain(x, y)) {
				setTerrain(TERRENO_DEFAULT_NOMBRE, x, y);
			}
		}
	}
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

shared_ptr<EntityFactory> ABoard::createEntityFactory(string name, r2 size, double speed, int sight_radius, bool solid, int capacity, std::string behaviour) {
	shared_ptr<EntityFactory> pFactory;
	if (behaviour == "resource") {
		pFactory = make_shared<ResourceFactory>(name, size, sight_radius, solid, capacity, *this);
	}else if (behaviour == "terrain") {
		pFactory = make_shared<TerrainFactory>(name, size, sight_radius, solid, *this);
	}else if(behaviour == "unit") {
		pFactory = make_shared<UnitFactory>(name, size, speed, sight_radius, solid, 100, *this); // TODO: Traer Health
	 } else if(behaviour == "worker") {
		pFactory = make_shared<WorkerFactory>(name, size, speed, sight_radius, solid, 100, *this); // TODO: Traer Health
	 } else if(behaviour == "king") {
		pFactory = make_shared<KingFactory>(name, size, speed, sight_radius, solid, 100, *this); // TODO: Traer Health
	 } else if (behaviour == "building") {
		 pFactory = make_shared<BuildingFactory>(name, size, sight_radius, solid, 100, *this); // TODO: Traer Health
	 } else if(behaviour == "producer_building") {
	 //TODO: products
		pFactory = make_shared<ProducerBuildingFactory>(name, size, sight_radius, solid, 100, *this); // TODO: Traer Health
	 } else if(behaviour == "town_center") {
	 //TODO: products
		pFactory = make_shared<TownCenterFactory>(name, size, sight_radius, solid, 100, *this); // TODO: Traer Health
	 } else if(behaviour == "flag") {
		pFactory = make_shared<FlagFactory>(name, size, sight_radius, solid, 100, *this); // TODO: Traer Health
	 }
	 else {
		 pFactory = make_shared<UnitFactory>(name, size, speed, sight_radius, solid, 100, *this); // TODO: Traer Health
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

void ABoard::mapEntities(EntityFunction fun) {
	for(size_t i = 0; i < entities.size(); i++) {
		fun(entities[i]);
	}
}

std::vector<std::shared_ptr<Entity>> ABoard::selectEntities(EntityPredicate pred) {
	std::vector<std::shared_ptr<Entity>> ret;
	mapEntities([&ret, pred] (std::shared_ptr<Entity> e) {if (pred(e)) {ret.push_back(e);};});
	return ret;
}

std::vector<shared_ptr<Entity>> ABoard::selectEntities(rectangle r) {
	return selectEntities([r](shared_ptr<Entity> e) {
			return rectangle(e->getPosition(), e->size).intersects(r); });
}

std::shared_ptr<Entity> ABoard::findEntity(EntityPredicate pred) {
	shared_ptr<Entity> ret = nullptr;
	for(size_t i = 0; i < entities.size(); i++) {
		if(pred(entities[i])) {
			ret = entities[i];
			break;
		}
	}
	return ret;
}

shared_ptr<Entity> ABoard::findEntity(size_t id) {
	return findEntity([id](shared_ptr<Entity> e) {
			return e?e->getId() == id:false; });
}

shared_ptr<Entity> ABoard::findEntity(rectangle r) {
	return findEntity([r](shared_ptr<Entity> e) {
			return rectangle(e->getPosition(), e->size).intersects(r); });
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

void ABoard::setState(enum ABoard::BoardState newState) {
	state = newState;
}

bool ABoard::isRunning() {
	return state == BoardState::running;
}

void ABoard::start() {
	if(!started) {
		started = true;
		th = thread(&ABoard::run, this);
	}
}

void ABoard::run() {
	while (isRunning()) {
		update();
		timer.elapse(dt);
	}
}

void ABoard::update() {
	if(!isRunning()) {
		return;
	}
	frame++;
	for(size_t i = 0; i < entities.size();) {
		if (entities[i]->getDeletable()) {
			game.notifyDeath(entities[i]->getId());
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

SmartBoard::SmartBoard(Game& game, RulesetParser& rulesetParser, ScenarioParser& scenarioParser) :
	ABoard(game,
			rulesetParser,
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
	fillTerrain();

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
			e->execute(command);
		}
	}
}

void SmartBoard::execute(MoveCommand& command) {
	auto e = findEntity(command.entityId);
	if (e) {
		if (e->owner.getAlive()) {
			e->execute(command);
		}
	}
}

