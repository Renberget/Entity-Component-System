#include "World.hpp"

Entity World::create(const ComponentMask& mask)
{
	return createEntity(mArchetypeManager.getOrCreateArchetype(mask));
}

void World::create(size_t count, const ComponentMask& mask)
{
	Archetype& archetype = mArchetypeManager.getOrCreateArchetype(mask);
	for (size_t i = 0; i < count; ++i)
		createEntity(archetype);
}

void World::create(std::span<Entity> entities, const ComponentMask& mask)
{
	Archetype& archetype = mArchetypeManager.getOrCreateArchetype(mask);
	for (Entity& entity : entities)
		entity = createEntity(archetype);
}

void World::remove(Entity entity)
{
	swapAndPop(entity);
	mRecords.remove(entity);
}

void World::remove(std::span<const Entity> entities)
{
	for (Entity entity : entities)
		remove(entity);
}

Entity World::clone(Entity entity)
{
	Entity clone = createEntity(*mRecords[entity].archetype);
	mRecords[entity].archetype->copyComponents(mRecords[entity].index, mRecords[clone].index);
	return clone;
}

void World::clone(Entity entity, std::span<Entity> clones)
{
	for (Entity& clone : clones)
	{
		clone = createEntity(*mRecords[entity].archetype);
		mRecords[entity].archetype->copyComponents(mRecords[entity].index, mRecords[clone].index);
	}
}

Entity World::instantiate(Entity entity)
{
	ComponentMask mask = mRecords[entity].archetype->mask();
	mask.remove<Prefab>();
	Archetype& archetype = mArchetypeManager.getOrCreateArchetype(mask);
	Entity instance = createEntity(archetype);
	mRecords[entity].archetype->copyComponentsTo(archetype, mRecords[entity].index, mRecords[instance].index);
	return instance;
}

void World::instantiate(Entity entity, std::span<Entity> instances)
{
	if (instances.empty()) [[unlikely]]
		return;

	ComponentMask mask = mRecords[entity].archetype->mask();
	mask.remove<Prefab>();
	Archetype& newArchetype = mArchetypeManager.getOrCreateArchetype(mask);

	instances[0] = createEntity(newArchetype);
	newArchetype.copyComponentsTo(newArchetype, mRecords[entity].index, mRecords[instances[0]].index);
	for (size_t i = 1; i < instances.size(); ++i)
	{
		instances[i] = createEntity(newArchetype);
		newArchetype.copyComponents(mRecords[instances[0]].index, mRecords[instances[i]].index);
	}
}

void World::addComponent(Entity entity, ComponentId id)
{
	ComponentMask mask = mRecords[entity].archetype->mask();
	mask.set(id, true);
	moveEntity(entity, mask);
}

void World::addComponents(Entity entity, std::span<const ComponentId> ids)
{
	ComponentMask mask = mRecords[entity].archetype->mask();
	for (ComponentId id : ids)
		mask.set(id, true);
	moveEntity(entity, mask);
}

void World::removeComponent(Entity entity, ComponentId id)
{
	ComponentMask mask = mRecords[entity].archetype->mask();
	mask.set(id, false);
	moveEntity(entity, mask);
}

void World::removeComponents(Entity entity, std::span<const ComponentId> ids)
{
	ComponentMask mask = mRecords[entity].archetype->mask();
	for (ComponentId id : ids)
		mask.set(id, false);
	moveEntity(entity, mask);
}

const ComponentMask& World::getMask(Entity entity) const
{
	return mRecords[entity].archetype->mask();
}

Query& World::query(const QueryFilters& filters)
{
	return mArchetypeManager.getOrCreateQuery(filters);
}

void World::clear()
{
	mArchetypeManager.clear();
}

void World::swapAndPop(Entity entity)
{
	Entity entityToSwap = mRecords[entity].archetype->swapAndPop(mRecords[entity].index);
	if (entityToSwap != Entity::Invalid) [[likely]]
		mRecords[entity].index = mRecords[entityToSwap].index;
}

Entity World::createEntity(Archetype& archetype)
{
	return mRecords.insert(archetype);
}

void World::moveEntity(Entity entity, const ComponentMask& newMask)
{
	Archetype& newArchetype = mArchetypeManager.getOrCreateArchetype(newMask);
	size_t newArchetypeIndex = newArchetype.addEntity(entity);
	mRecords[entity].archetype->copyComponentsTo(newArchetype, mRecords[entity].index, newArchetypeIndex);
	swapAndPop(entity);
	mRecords[entity].archetype = &newArchetype;
	mRecords[entity].index = newArchetypeIndex;
}
