#ifndef SRC_MAIN_ESCENARIO_ESCENARIO_H_
#define SRC_MAIN_ESCENARIO_ESCENARIO_H_
//-----------------------------------------------------------------------------
#include <string>
//-----------------------------------------------------------------------------
class Escenario {

private:
	int x;
	int y;
	std::string name;
	int fps;
	int delay;
	std::string image;
public:
	Escenario();
	virtual ~Escenario();
};
//-----------------------------------------------------------------------------
#endif /* SRC_MAIN_ESCENARIO_ESCENARIO_H_ */
//-----------------------------------------------------------------------------
