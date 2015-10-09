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
	auto tc = parser.getConfiguracion();
	auto te = parser.getEscenario();
	board = make_shared<Board>(te.size_x, te.size_y, tc.dt, parser); 
	auto tp = parser.getPantalla();
	gameWindow = make_shared<GameWindow>(*this, board->findPlayer("Franceses"), tp.ancho, tp.alto, tc.margen_scroll, tc.velocidad_scroll);

	for(auto& t : parser.getTiposEntidades()) {
		gameWindow->addSpriteSheet(t.nombre, t.imagen, t.pixel_ref_x, t.pixel_ref_y, t.alto_sprite, t.ancho_sprite,  t.cantidad_sprites, t.fps, t.delay);
	}

	for(auto& t : parser.getTiposTerrenos()) {
		gameWindow->addSpriteSheet(t.nombre, t.imagen, t.pixel_ref_x, t.pixel_ref_y, t.alto_sprite, t.ancho_sprite,  t.cantidad_sprites, t.fps, t.delay);
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

