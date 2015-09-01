#include "entidades_manager.h"
//-----------------------------------------------------------------------------
EntidadesManager* EntidadesManager::entidadesManager = 0;
//-----------------------------------------------------------------------------
EntidadesManager::EntidadesManager() {
	// TODO Auto-generated constructor stub

}
//-----------------------------------------------------------------------------
EntidadesManager* EntidadesManager::getInstance()
{
	if (EntidadesManager::entidadesManager == 0)
	{
		entidadesManager = new EntidadesManager();
		return entidadesManager;
	}
	return entidadesManager;
}
//-----------------------------------------------------------------------------
EntidadesManager::~EntidadesManager() {
	// TODO Auto-generated destructor stub
}
//-----------------------------------------------------------------------------
