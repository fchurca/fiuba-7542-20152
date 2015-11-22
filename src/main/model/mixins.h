#ifndef __MODEL_MIXINS_H__
#define __MODEL_MIXINS_H__

#include <cstdint>

class DeletableMixin {
protected:
	bool deletable;
	DeletableMixin();
public:
	virtual ~DeletableMixin();
	virtual void setDeletable();
	virtual bool getDeletable();
};

class FrameMixin {
protected:
	std::size_t frame;
	FrameMixin();
public:
	virtual ~FrameMixin();
	virtual std::size_t getFrame();
};

class IdMixin {
protected:
	std::size_t id;
	IdMixin();
public:
	virtual ~IdMixin();
	virtual std::size_t getId();
	virtual void setId(std::size_t newId);
};

#endif // __MODEL_MIXINS_H__
