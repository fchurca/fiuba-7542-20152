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
	board = make_shared<Board>(parser); 
	gameWindow = make_shared<GameWindow>(*this, board->findPlayer("Franceses"), parser);

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

		Logger::getInstance()->flush();
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

