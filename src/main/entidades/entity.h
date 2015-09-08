
#ifndef SRC_MAIN_ENTIDADES_ENTIDAD_H_
#define SRC_MAIN_ENTIDADES_ENTIDAD_H_
//-----------------------------------------------------------------------------
#include <string>
//-----------------------------------------------------------------------------
class Entidad {

private:
	int x;
	int y;
	std::string name;
	int delay;
	int fps;
	std::string image;
public:
	Entidad();
	virtual ~Entidad();
};
//-----------------------------------------------------------------------------
#endif /* SRC_MAIN_ENTIDADES_ENTIDAD_H_ */
//-----------------------------------------------------------------------------
