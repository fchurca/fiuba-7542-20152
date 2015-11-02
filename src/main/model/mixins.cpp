#include "mixins.h"

using namespace std;

IdMixin::IdMixin() {}

size_t IdMixin::getId() {
	return id;
}

void IdMixin::setId(size_t newId) {
	id = newId;
}

