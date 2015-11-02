#ifndef __MODEL_MIXINS_H__
#define __MODEL_MIXINS_H__

#include <cstdint>

class IdMixin {
	protected:
		std::size_t id;
		IdMixin();
	public:
		std::size_t getId();
		void setId(std::size_t newId);
};

#endif // __MODEL_MIXINS_H__
