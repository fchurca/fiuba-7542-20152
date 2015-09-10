#include "board.h"
#include <iostream>

using namespace std;

//-----------------------------------------------------------------------------
void Board::buildBoard(ParserYAML* parser) {
	createEntity("Dummy", 0, 0);
}

void Board::createEntity(std::string name, double x, double y) {
	entities.push_back(new Entity(name, *this, x, y));
}

Board::Board(int sizeX, int sizeY) : sizeX(sizeX), sizeY(sizeY) {
	cerr << "Creating board " << this << " of size " << sizeX << "x" << sizeY << endl;
}

Board::~Board() {
	cerr << "Killing Board " << this << endl;
	while (!entities.empty()) {
		auto e = entities.back();
		entities.pop_back();
		delete e;
	}
}

void Board::update() {
	cerr << "Board " << this << " updating" << endl;
	for(auto& e : entities) {
		e->update();
	}
	cerr << "Board " << this << " updated" << endl;
}

