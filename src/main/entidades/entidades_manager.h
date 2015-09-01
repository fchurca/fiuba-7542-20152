#ifndef SRC_MAIN_ENTIDADES_ENTIDADES_MANAGER_H_
#define SRC_MAIN_ENTIDADES_ENTIDADES_MANAGER_H_
//-----------------------------------------------------------------------------
class EntidadesManager {
private:
	EntidadesManager();

public:
	static EntidadesManager* entidadesManager;
	static EntidadesManager* getInstance();
	virtual ~EntidadesManager();
};
//-----------------------------------------------------------------------------
#endif /* SRC_MAIN_ENTIDADES_ENTIDADES_MANAGER_H_ */
//-----------------------------------------------------------------------------
