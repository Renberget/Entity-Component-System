#pragma once
#include "Component.hpp"
#include <array>
#include <assert.h>

using ComponentId = uint32_t;

constexpr size_t MaxComponents = 128;

class ComponentManager
{
public:
	ComponentManager() = delete;
	template<component T>
	static ComponentId getId()
	{
		static const ComponentId id = add<T>();
		return id;
	}

	static size_t getSize(ComponentId id)
	{
		assert(id < sComponentCount);
		return sComponentsSize[id];
	}

private:
	static std::array<size_t, MaxComponents> sComponentsSize;
	static ComponentId sComponentCount;

	template<component T>
	static ComponentId add()
	{
		assert(sComponentCount < MaxComponents);
		sComponentsSize[sComponentCount] = sizeof(T);
		return sComponentCount++;
	}
};
