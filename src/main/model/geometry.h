#ifndef _MODEL_GEOMETRY_H_
#define _MODEL_GEOMETRY_H_

#include <cmath>

double clip(double x, double min, double max);

double interpolate(double x, double xa, double xb, double ya, double yb);


class r2 {
	public:
		double x;
		double y;

		r2();
		r2(double x, double y);
		static r2 fromPolar(double theta, double r);

		r2 operator - () ;

		r2 operator / (double a);
		r2 operator + (r2 b);
		r2 operator - (r2 b);

		r2 & operator += (r2 b);
		r2 & operator -= (r2 b);

		bool operator == (r2 b);
		bool operator != (r2 b);

		double sqLength();
		double length();
};


class rectangle {
	public:
		r2 position;
		r2 size;

		rectangle();
		explicit rectangle(r2 position, r2 size);
		bool intersects(rectangle other);

		static rectangle box(rectangle a, rectangle b);
};

#endif // _MODEL_GEOMETRY_H_

