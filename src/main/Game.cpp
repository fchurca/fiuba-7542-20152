#include "Game.h"

Game::Game(){
}

Game::~Game(){
}

bool Game::endOfGame(){
	return true;
}

void Game::render(){
	return;
}

void Game::update(){
	return;
}

void Game::processInput(){
	return;
}

int Game::start(){
	while (!endOfGame())
	{
		processInput();
		update();
		render();
		//sleep();
	}
	return 0;
}
