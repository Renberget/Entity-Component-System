#pragma once
#include "ComponentManager.hpp"
#include <bitset>

class ComponentMask : public std::bitset<MaxComponents>
{
public:
	template<component... Ts>
	constexpr static ComponentMask create()
	{
		ComponentMask mask;
		(mask.add<Ts>(), ...);
		return mask;
	}

	constexpr ComponentMask() = default;

	template<component... Ts>
	constexpr void add()
	{
		(set(ComponentManager::getId<Ts>(), true), ...);
	}

	template<component... Ts>
	constexpr void remove()
	{
		(set(ComponentManager::getId<Ts>(), false), ...);
	}

	template<component... Ts>
	bool has() const
	{
		return (test(ComponentManager::getId<Ts>()) && ...);
	}
};

template<>
struct std::hash<ComponentMask>
{
	size_t operator()(const ComponentMask& mask) const noexcept
	{
		return std::hash<std::bitset<MaxComponents>>{}(mask);
	}
};