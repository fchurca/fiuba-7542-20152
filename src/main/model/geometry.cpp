#include "geometry.h"

double clip(double x, double min, double max) {
	return (x > max) ?  max :
		(x < min) ? min :
		x;
}

double interpolate(double x, double xa, double xb, double ya, double yb) {
	return ya + (yb - ya) * (x - xa) / (xb - xa);
}

r2::r2():x(0),y(0){}

r2::r2(double x, double y):x(x),y(y){};

r2 r2::fromPolar(double theta, double r) {
	return {cos(theta)*r,sin(theta)*r};
}

r2 r2::operator - () {
	return {-x, -y};
}

r2 r2::operator / (double a) {
	return {x/a, y/a};
}

r2 r2::operator + (r2 b) {
	return {x + b.x, y + b.y};
}

r2 r2::operator - (r2 b) {
	return *this + -b;
}

r2 & r2::operator += (r2 b) {
	x += b.x;
	y += b.y;
	return *this;
}

r2 & r2::operator -= (r2 b) {
	return *this += -b;
}

bool r2::operator == (r2 b) {
	return (x == b.x) && (y == b.y);
}

bool r2::operator != (r2 b) {
	return ! (*this == b);
}

double r2::sqLength() {
	return x*x + y*y;
}

double r2::length() {
	return sqrt(sqLength());
}


rectangle::rectangle() {}

rectangle::rectangle(r2 position, r2 size) :
	position(position), size(size)
{}

bool rectangle::intersects(rectangle other) {
	return position.x < other.position.x + other.size.x &&
		position.x + size.x > other.position.x &&
		position.y < other.position.y + other.size.y &&
		position.y + size.y > other.position.y;
}

rectangle rectangle::box(rectangle a, rectangle b) {
	r2
		pa = a.position,
		pb = b.position;
	bool
		la = pa.x < pb.x,
		ua = pa.y < pb.y;
	rectangle &
		l = la ? a : b,
		r = la ? b : a,
		u = ua ? a : b,
		d = ua ? b : a;
	return rectangle(
			r2(l.position.x, u.position.y),
			r2(r.position.x - l.position.x + r.size.x,
				d.position.y - u.position.y + d.size.y));
}

rectangle rectangle::box(r2 a, r2 b, r2 margin) {
	return box(rectangle(a, margin), rectangle(b, margin));
}

