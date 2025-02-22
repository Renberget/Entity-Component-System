#pragma once
#include "Archetype.hpp"
#include <deque>

struct Record
{
	Archetype* archetype;
	union
	{
		size_t index;
		Entity nextFree;
	};
};

class Records : public Uncopyable
{
public:
	Entity insert(Archetype& archetype);
	void remove(Entity entity);
	void clear();
	Record& operator[](Entity entity);
	const Record& operator[](Entity entity) const;

private:
	[[nodiscard]] bool isValid(Entity entity) const;

	std::deque<Record> mRecords;
	Entity mLastFreeEntity = Entity::Invalid;
};