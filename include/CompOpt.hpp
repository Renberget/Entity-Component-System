#pragma once
#include "CompPtr.hpp"

template<component T>
class CompOpt : public CompPtr<T>
{
public:
#ifdef NDEBUG
	explicit CompOpt(T* ptr) : CompPtr(ptr) {}
#else
	explicit CompOpt(T* ptr, uint32_t& counter) : CompPtr(ptr, counter) {}
#endif

	[[nodiscard]] bool notNull() const
	{
		return this->mPtr;
	}
};
