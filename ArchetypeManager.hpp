#pragma once
#include "Query.hpp"

class ArchetypeManager : public Uncopyable
{
public:
	[[nodiscard]] Archetype& getOrCreateArchetype(const ComponentMask& mask);
	[[nodiscard]] Query& getOrCreateQuery(const QueryFilters& filters);
	void clear();

private:
	void updateQueries();

	std::unordered_map<ComponentMask, std::unique_ptr<Archetype>> mMaskToArchetype;
	std::unordered_map<QueryFilters, std::unique_ptr<Query>> mFiltersToQuery;
};