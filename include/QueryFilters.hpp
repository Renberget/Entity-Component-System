#pragma once
#include "Archetype.hpp"

class QueryFilters
{
public:
	template<component... Ts>
	constexpr QueryFilters& require()
	{
		mRequiredMask.add<Ts...>();
		return *this;
	}

	template<component... Ts>
	constexpr QueryFilters& exclude()
	{
		mExcludedMask.add<Ts...>();
		return *this;
	}

	bool operator==(const QueryFilters& other) const
	{
		return mRequiredMask == other.mRequiredMask && mExcludedMask == other.mExcludedMask;
	}

	inline const ComponentMask& requiredMask() const { return mRequiredMask; }
	inline const ComponentMask& excludedMask() const { return mExcludedMask; }

private:
	ComponentMask mRequiredMask;
	ComponentMask mExcludedMask;
};

template<>
struct std::hash<QueryFilters>
{
	size_t operator()(const QueryFilters& filters) const noexcept
	{
		return std::hash<ComponentMask>{}(filters.requiredMask()) ^ std::hash<ComponentMask>{}(filters.excludedMask());
	}
};