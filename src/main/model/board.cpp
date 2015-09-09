#include "board.h"
#include <iostream>

using namespace std;

//-----------------------------------------------------------------------------
void Board::buildBoard(ParserYAML* parser) {
	entities.push_back(new Entity("Dummy"));
	cerr << "Adding a new Dummy entity" << endl;
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

