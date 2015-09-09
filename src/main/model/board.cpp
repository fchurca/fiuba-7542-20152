#include "board.h"
#include <iostream>

using namespace std;

//-----------------------------------------------------------------------------
void Board::buildBoard(ParserYAML* parser) {
}

Board::Board() {
	entities.push_back(new Entity("Dummy"));
	cerr << "Adding a new Dummy entity" << endl;
}

Board::~Board() {
	cerr << "Killing board " << this << endl;
	for(Entity* & e : entities) {
		cerr << "Killing entity " << e->name << endl;
		delete e;
		e = NULL;
	}
}

void Board::update() {
	/*cerr << "Board " << this << " updating" << endl;
	// TODO: hacer vivir a las entidades
	cerr << "Board " << this << " updated" << endl;*/
}

