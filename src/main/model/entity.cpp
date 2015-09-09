#include "entity.h"
#include <iostream>
//-----------------------------------------------------------------------------
Entity::Entity(std::string newName) : name(newName) {}
//-----------------------------------------------------------------------------
Entity::~Entity() {
	std::cerr << "Killing Entity " << name << " " << this << std::endl;
}
//-----------------------------------------------------------------------------

void Entity::update() {
	std::cerr << "Entity " << this << " is alive" << std::endl;
}
