#pragma once
#include "ChunkAllocator.hpp"
#include "ComponentMask.hpp"
#include "CompOpt.hpp"
#include <functional>
#include <span>

class Archetype : public Uncopyable
{
public:
	explicit Archetype(const ComponentMask& mask);
	Archetype(Archetype&&) noexcept = default;
	Archetype& operator=(Archetype&&) noexcept = default;
	[[nodiscard]] inline const ComponentMask& mask() const { return mMask; }
	[[nodiscard]] inline std::span<const ComponentId> componentIds() const { return mComponentIds; }
	[[nodiscard]] size_t getCount() const;

	size_t addEntity(Entity entity);
	//Only way to remove an entity, returns the swapped entity
	[[nodiscard]] Entity swapAndPop(size_t index);
	void clear();

	void copyComponents(size_t srcIndex, size_t dstIndex);
	void copyComponentsTo(Archetype& newArchetype, size_t srcIndex, size_t dstIndex);

	template<component T>
	[[nodiscard]] CompPtr<T> getComponent(size_t index);
	template<typename F>
	void forEach(F&& callback);

private:
	template<typename F, typename R, typename... Args>
	void forEach(F&& callback, const std::function<R(Args...)>&);

	template<typename T>
	[[nodiscard]] T getData(Chunk& chunk, size_t index);
	[[nodiscard]] std::byte* getData(ComponentId id, size_t index);
	[[nodiscard]] Entity& getEntity(size_t index);

	std::array<size_t, MaxComponents> mComponentOffsets;
	std::vector<ChunkPtr> mChunks;
	std::vector<ComponentId> mComponentIds;
	const ComponentMask mMask;
	size_t mChunkCapacity;
	size_t mLastChunkCount = 0;

#ifndef NDEBUG
	uint32_t mPtrCount = 0;
#endif
};

#include "Archetype.inl"
