#pragma once
#include "QueryFilters.hpp"

class Query : public Uncopyable
{
public:
	Query(const QueryFilters& filters);

	template<typename F>
	void forEach(F&& callback) const
	{
		for (Archetype* archetype : mArchetypes)
			archetype->forEach(std::forward<F>(callback));
	}

	[[nodiscard]] size_t getCount() const;

private:
	void update(std::unordered_map<ComponentMask, std::unique_ptr<Archetype>>& maskToArchetype);

	const QueryFilters mFilters;
	std::vector<Archetype*> mArchetypes;

	friend class ArchetypeManager;
};
