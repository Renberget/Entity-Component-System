#include "Records.hpp"

Entity Records::insert(Archetype& archetype)
{
	Entity entity;
	if (mLastFreeEntity == Entity::Invalid)
	{
		entity = Entity(mRecords.size());
		assert(entity != Entity::Invalid && "Max entity count exceeded");
		mRecords.emplace_back();
	}
	else
	{
		entity = mLastFreeEntity;
		mLastFreeEntity = (*this)[mLastFreeEntity].nextFree;
	}
	mRecords[entity].archetype = &archetype;
	mRecords[entity].index = archetype.addEntity(entity);
	return entity;
}

void Records::remove(Entity entity)
{
	assert(isValid(entity));
#ifndef NDEBUG
	mRecords[entity].archetype = nullptr;
#endif
	mRecords[entity].nextFree = mLastFreeEntity;
	mLastFreeEntity = entity;
}

void Records::clear()
{
	mRecords.clear();
	mLastFreeEntity = Entity::Invalid;
}

Record& Records::operator[](Entity entity)
{
	assert(isValid(entity));
	return mRecords[entity];
}

const Record& Records::operator[](Entity entity) const
{
	assert(isValid(entity));
	return mRecords[entity];
}

bool Records::isValid(Entity entity) const
{
	if (entity < mRecords.size())
		return mRecords[entity].archetype;
	return false;
}
