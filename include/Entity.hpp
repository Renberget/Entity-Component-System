#pragma once
#include <concepts>
#include <cstdint>
#include <limits>

struct Entity
{
	static const Entity Invalid;

	using IdType = uint32_t;

	Entity() = default;
	template<std::integral T>
	constexpr explicit Entity(T index) : id(static_cast<IdType>(index)) {}
	operator IdType() const
	{
		return id;
	}

	IdType id;
};
