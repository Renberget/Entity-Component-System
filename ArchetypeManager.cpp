#include "ArchetypeManager.hpp"

Archetype& ArchetypeManager::getOrCreateArchetype(const ComponentMask& mask)
{
	auto [it, inserted] = mMaskToArchetype.insert({ mask, nullptr });
	if (inserted) [[unlikely]]
	{
		it->second = std::make_unique<Archetype>(mask);
		updateQueries();
	}
	return *it->second;
}

Query& ArchetypeManager::getOrCreateQuery(const QueryFilters& filters)
{
	auto [it, inserted] = mFiltersToQuery.insert({ filters, nullptr });
	if (inserted) [[unlikely]]
	{
		it->second = std::make_unique<Query>(filters);
		it->second->update(mMaskToArchetype);
	}
	return *it->second;
}

void ArchetypeManager::clear()
{
	for (auto& [mask, archetype] : mMaskToArchetype)
		archetype->clear();
	updateQueries();
}

void ArchetypeManager::updateQueries()
{
	for (auto it = mFiltersToQuery.begin(); it != mFiltersToQuery.end(); ++it)
		it->second->update(mMaskToArchetype);
}