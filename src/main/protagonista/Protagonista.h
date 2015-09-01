#ifndef SRC_MAIN_PROTAGONISTA_PROTAGONISTA_H_
#define SRC_MAIN_PROTAGONISTA_PROTAGONISTA_H_
//-----------------------------------------------------------------------------
#include <string>
//-----------------------------------------------------------------------------
class Protagonista {
	int x;
	int y;
	std::string name;
	int delay;
	int fps;
	std::string image;
public:
	Protagonista();
	virtual ~Protagonista();
};
//-----------------------------------------------------------------------------
#endif /* SRC_MAIN_PROTAGONISTA_PROTAGONISTA_H_ */
//-----------------------------------------------------------------------------
