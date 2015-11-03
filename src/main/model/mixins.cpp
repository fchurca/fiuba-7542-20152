#include "mixins.h"

using namespace std;


DeletableMixin::DeletableMixin() : deletable(false) {}

DeletableMixin::~DeletableMixin() {}

bool DeletableMixin::getDeletable() {
	return deletable;
}

void DeletableMixin::setDeletable() {
	deletable = true;;
}


FrameMixin::FrameMixin() : frame(0) {
}

FrameMixin::~FrameMixin() {}

size_t FrameMixin::getFrame() {
	return frame;
}


IdMixin::IdMixin() {}

IdMixin::~IdMixin() {}

size_t IdMixin::getId() {
	return id;
}

void IdMixin::setId(size_t newId) {
	id = newId;
}

