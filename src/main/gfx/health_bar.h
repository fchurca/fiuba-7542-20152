#ifndef __GFX_HEALTH_BAR_H__
#define __GFX_HEALTH_BAR_H__
#include <string>
#include "../model/entity.h"
class IsoView;

class HealthBar : public EntityVisitor {
private:
	IsoView& owner;
protected:
public:
	HealthBar(IsoView& owner);
	~HealthBar();
	virtual void visit(Entity& e);
	virtual void visit(Unit& e);
	virtual void visit(Building& e);
	virtual void visit(ProducerBuilding& e);
	virtual void visit(UnfinishedBuilding& e);

};
#endif // __GFX_HEALTH_BAR_H__