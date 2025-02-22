template<component... Ts>
Entity World::create()
{
	return create(ComponentMask::create<Ts...>());
}

template<component... Ts>
void World::create(size_t count)
{
	return create(count, ComponentMask::create<Ts...>());
}

template<component... Ts>
void World::create(std::span<Entity> entities)
{
	return create(entities, ComponentMask::create<Ts...>());
}

template<component T>
void World::addComponent(Entity entity)
{
	addComponent(entity, ComponentManager::getId<T>());
}

template<component T>
void World::addComponent(Entity entity, const T& component)
{
	ComponentMask mask = mRecords[entity].archetype->mask();
	mask.add<T>();
	moveEntity(entity, mask);
	mRecords[entity].archetype->getComponent<T>(mRecords[entity].index) = component;
}

template<component... Ts>
void World::addComponents(Entity entity)
{
	addComponents(entity, { ComponentManager::getId<Ts>()... });
}

template<component... Ts>
void World::addComponents(Entity entity, const Ts&... components)
{
	ComponentMask mask = mRecords[entity].archetype->mask();
	mask.add<Ts...>();
	moveEntity(entity, mask);
	((mRecords[entity].archetype->getComponent<Ts>(mRecords[entity].index) = components), ...);
}

template<component T>
void World::removeComponent(Entity entity)
{
	removeComponent(entity, ComponentManager::getId<T>());
}

template<component... Ts>
void World::removeComponents(Entity entity)
{
	removeComponents(entity, { ComponentManager::getId<Ts>()... });
}

template<component T>
CompPtr<T> World::getComponent(Entity entity)
{
	return mRecords[entity].archetype->getComponent<T>(mRecords[entity].index);
}

template<component... Ts>
std::tuple<CompPtr<Ts>...> World::getComponents(Entity entity)
{
	return std::tuple<CompPtr<Ts>...>(mRecords[entity].archetype->getComponent<Ts>(mRecords[entity].index)...);
}

