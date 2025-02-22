#include "Query.hpp"

Query::Query(const QueryFilters& filters) : mFilters(filters) {}

size_t Query::getCount() const
{
	size_t count = 0;
	for (const Archetype* archetype : mArchetypes)
		count += archetype->getCount();
	return count;
}

void Query::update(std::unordered_map<ComponentMask, std::unique_ptr<Archetype>>& maskToArchetype)
{
	for (auto it = maskToArchetype.begin(); it != maskToArchetype.end(); ++it)
	{
		if ((mFilters.requiredMask() & it->second->mask()) == mFilters.requiredMask()
			&& !(mFilters.excludedMask() & it->second->mask()).any())
		{
			mArchetypes.push_back(it->second.get());
		}
	}
}
