#ifndef _MODEL_GEOMETRY_H_
#define _MODEL_GEOMETRY_H_

#include <cmath>

double clip(double x, double min, double max);

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

		bool operator == (r2 b);
		bool operator != (r2 b);
};

#endif // _MODEL_GEOMETRY_H_

