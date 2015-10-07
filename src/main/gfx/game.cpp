#include <sstream>

#include "game.h"
#include "game_window.h"
#include "../parser_yaml/parser_yaml.h"

Game::Game() :
	exit_p(false), restart_p(false)
{
	init();
}

Game::~Game(){
	std::stringstream message;
	message << "Killing Game " << this;
	Logger::getInstance()->writeInformation(message.str());
	clear();
}

void Game::clear() {
	board = nullptr;
	gameWindow = nullptr;
}

void Game::init(){
	std::stringstream message;
	message << "Creating Game " << this;
	Logger::getInstance()->writeInformation(message.str());

	clear();
	restart_p = false;

	ParserYAML parser(CONFIG_FILE_PATH);
	parser.parse();
	auto te = parser.getEscenario();
	auto tc = parser.getConfiguracion();
	board = make_shared<Board>(te.size_x, te.size_y, tc.dt); 
	auto tp = parser.getPantalla();
	gameWindow = make_shared<GameWindow>(*this, board->findPlayer("Franceses"), tp.ancho, tp.alto, tc.margen_scroll, tc.velocidad_scroll);

	for(auto& t : parser.getTiposEntidades()) {
		gameWindow->addSpriteSheet(t.nombre, t.imagen, t.pixel_ref_x, t.pixel_ref_y, t.alto_sprite, t.ancho_sprite,  t.cantidad_sprites, t.fps, t.delay);
		board->createEntityFactory(t.nombre, {t.ancho_base, t.alto_base}, tc.vel_personaje, t.sight_radius); // LA VELOCIDAD DEBERIA IR SOLO AL PROTAGONISTA
	}

	for(auto& t : parser.getTiposTerrenos()) {
		gameWindow->addSpriteSheet(t.nombre, t.imagen, t.pixel_ref_x, t.pixel_ref_y, t.alto_sprite, t.ancho_sprite,  t.cantidad_sprites, t.fps, t.delay);
		board->createEntityFactory(t.nombre, {t.ancho_base, t.alto_base}, 0, 0); 
	}

	for(auto& t : te.terrenos) {
		board->setTerrain(t.tipoEntidad, t.pos_x, t.pos_y);
	}

	board->init();

	// TODO: Levantar jugadores/facciones
	if(!board->createEntity(te.protagonista.tipoEntidad, "Franceses", {(double)te.protagonista.pos_x, (double)te.protagonista.pos_y})){
		Logger::getInstance()->writeInformation("Se crea un protagonista default");
		board->createEntity(PROTAGONISTA_DEFAULT_NOMBRE, "Franceses", {PROTAGONISTA_DEFAULT_POSX, PROTAGONISTA_DEFAULT_POSY});
	}

	// TODO: Revisar, por ahora todo el resto va para Gaia
	for(auto& t : te.entidades) {
		board->createEntity(t.tipoEntidad, DEFAULT_PLAYER_NAME, {(double)t.pos_x,(double)t.pos_y});
	}
	gameWindow->init();
}

void Game::start() {
	while (!exit_p) {
		if(restart_p) {
			init();
		}
		GameTimer::update();
		gameWindow->update(); // Controller
		board->update(); // Model
		gameWindow->render(); // View

		if (!GameTimer::wait(GameTimer::getCurrent() + board->dt)) {
			Logger::getInstance()->writeInformation("Estamos laggeando!");
		}
	}
}

void Game::restart() {
	restart_p = true;
}

void Game::exit() {
	exit_p = true;
}

