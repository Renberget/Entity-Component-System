#pragma once
#include "ArchetypeManager.hpp"
#include "Records.hpp"

class World : public Uncopyable
{
public:
	//Create a new entity
	Entity create(const ComponentMask& mask = ComponentMask());
	//Create new entities
	void create(size_t count, const ComponentMask& mask = ComponentMask());
	//Fill entities with newly created entities
	void create(std::span<Entity> entities, const ComponentMask& mask = ComponentMask());
	//Create a new entity using component types
	template<component... Ts>
	Entity create();
	//Create new entities
	template<component... Ts>
	void create(size_t count);
	//Fill entities with newly created entities
	template<component... Ts>
	void create(std::span<Entity> entities);

	//Remove an entity and its components, invalidate references in the same archetype
	void remove(Entity entity);
	//Remove entities and their components, invalidate references in the same archetype
	void remove(std::span<const Entity> entities);

	//Copy components of an entity into a new one
	Entity clone(Entity entity);
	//Copy components of an entity into new ones and write them to clones
	void clone(Entity entity, std::span<Entity> clones);

	//Clone an entity but removes the prefab component
	Entity instantiate(Entity entity);
	//Clone an entity but removes the prefab component
	void instantiate(Entity entity, std::span<Entity> instances);

	//Add an unitialized component to an entity, invalidate references in the same archetype
	void addComponent(Entity entity, ComponentId id);
	//Add an unitialized component to an entity, invalidate references in the same archetype
	template<component T>
	void addComponent(Entity entity);
	//Add an component to an entity, invalidate references in the same archetype
	template<component T>
	void addComponent(Entity entity, const T& component);

	//Add unitialized components to an entity, invalidate references in the same archetype
	void addComponents(Entity entity, std::span<const ComponentId> ids);
	//Add unitialized components to an entity, invalidate references in the same archetype
	template<component... Ts>
	void addComponents(Entity entity);
	//Add components to an entity, invalidate references in the same archetype
	template<component... Ts>
	void addComponents(Entity entity, const Ts&... components);

	//Remove a component from an entity, invalidate references in the same archetype
	void removeComponent(Entity entity, ComponentId id);
	//Remove a component from an entity, invalidate references in the same archetype
	template<component T>
	void removeComponent(Entity entity);

	//Remove components from an entity, invalidate references in the same archetype
	void removeComponents(Entity entity, std::span<const ComponentId> ids);
	//Remove components from an entity, invalidate references in the same archetype
	template<component... Ts>
	void removeComponents(Entity entity);

	//Return a reference to a component of an entity
	template<component T>
	[[nodiscard]] CompPtr<T> getComponent(Entity entity);
	//Return a tuple of references to components of an entity
	template<component... Ts>
	[[nodiscard]] std::tuple<CompPtr<Ts>...> getComponents(Entity entity);

	//Return the component mask of an entity
	[[nodiscard]] const ComponentMask& getMask(Entity entity) const;

	//Return a cached query or a new one if it's the first time
	[[nodiscard]] Query& query(const QueryFilters& filters);

	//Remove all entities, does not remove archetypes and queries
	void clear();

private:
	Entity createEntity(Archetype& archetype);
	void swapAndPop(Entity entity);
	void moveEntity(Entity entity, const ComponentMask& newMask);

	ArchetypeManager mArchetypeManager;
	Records mRecords;
};

#include "World.inl"
