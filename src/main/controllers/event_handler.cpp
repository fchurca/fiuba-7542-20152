#include "event_handler.h"

EventHandler::EventHandler(){
}

EventHandler::~EventHandler(){
}

SDL_Event * EventHandler::getEvent(){
	return &(this->evento);
}

EventHandler * EventHandler::instance = 0;
EventHandler * EventHandler::getInstance() {
	if (!instance)
		instance = new EventHandler();
	return instance;
}