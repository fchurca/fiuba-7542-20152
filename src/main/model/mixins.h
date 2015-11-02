#ifndef __MODEL_MIXINS_H__
#define __MODEL_MIXINS_H__

#include <cstdint>

class FrameMixin {
	protected:
		std::size_t frame;
		FrameMixin();
	public:
		std::size_t getFrame();
};

class IdMixin {
	protected:
		std::size_t id;
		IdMixin();
	public:
		virtual std::size_t getId();
		virtual void setId(std::size_t newId);
};

#endif // __MODEL_MIXINS_H__
