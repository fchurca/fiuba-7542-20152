#include "geometry.h"

double clip(double x, double min, double max) {
	return (x > max) ?  max :
		(x < min) ? min :
		x;
}

r2::r2(){}

r2::r2(double x, double y):x(x),y(y){};

r2 r2::fromPolar(double theta, double r) {
	return r2(cos(theta)*r,sin(theta)*r);
}

r2 r2::operator - () {
	return r2(-x, -y);
}

r2 r2::operator / (double a) {
	return r2(x/a, y/a);
}

r2 r2::operator + (r2 b) {
	return r2(x + b.x, y + b.y);
}

r2 r2::operator - (r2 b) {
	return *this + -b;
}

r2 & r2::operator += (r2 b) {
	x += b.x;
	y += b.y;
	return *this;
}

bool r2::operator == (r2 b) {
	return (x == b.x) && (y == b.y);
}

bool r2::operator != (r2 b) {
	return ! (*this == b);
}

