#include "Archetype.hpp"

Archetype::Archetype(const ComponentMask& mask) : mMask(mask)
{
	//Compute stride and get component ids
	size_t stride = sizeof(Entity);
	for (size_t i = 0; i < mask.size(); ++i)
	{
		if (mask.test(i))
		{
			ComponentId id = static_cast<ComponentId>(i);
			mComponentIds.emplace_back(id);
			stride += ComponentManager::getSize(id);
		}
	}
	mComponentIds.shrink_to_fit();
	assert(stride <= sizeof(Chunk) && "Size of components exceed chunk size");

	//Compute capacity of a chunk
	mChunkCapacity = 0;
	size_t bytesToSplit = sizeof(Chunk);
	size_t addedCapacity = sizeof(Chunk) / stride;
	do
	{
		size_t componentSize = sizeof(Entity);
		for (size_t i = 0; i < mComponentIds.size(); ++i)
		{
			if (i != 0)
				componentSize = ComponentManager::getSize(mComponentIds[i - 1]);

			size_t addedComponentBytes = bytesToSplit * componentSize / stride;
			addedComponentBytes -= addedComponentBytes % Chunk::Alignment;
			addedCapacity = std::min(addedCapacity, addedComponentBytes / componentSize);
		}
		bytesToSplit -= addedCapacity * stride;
		mChunkCapacity += addedCapacity;

	} while (addedCapacity);

	assert(mChunkCapacity && "Can't fit any element in a chunk");

	//Set offset for each component
	size_t componentSize = sizeof(Entity);
	size_t offset = 0;
	for (size_t i = 0; i < mComponentIds.size(); ++i)
	{
		if (i != 0) [[likely]]
			componentSize = ComponentManager::getSize(mComponentIds[i - 1]);

		offset += mChunkCapacity * componentSize;
		offset += Chunk::Alignment - offset % Chunk::Alignment;
		mComponentOffsets[mComponentIds[i]] = offset;
	}
}

size_t Archetype::getCount() const
{
	return mChunks.size() ? ((mChunks.size() - 1) * mChunkCapacity + mLastChunkCount) : 0;
}

size_t Archetype::addEntity(Entity entity)
{
	size_t index = getCount();
	if (mLastChunkCount == mChunkCapacity || mChunks.empty()) [[unlikely]]
	{
		mChunks.push_back(ChunkAllocator::allocate());
		mLastChunkCount = 0;
	}
	++mLastChunkCount;
	getEntity(index) = entity;
	return index;
}

Entity Archetype::swapAndPop(size_t index)
{
	assert(mPtrCount == 0 && "There must be no component pointers to this archetype to swap and pop");

	size_t count = getCount();
	if (count == 0) [[unlikely]]
		return Entity::Invalid;

	size_t lastIndex = count - 1;
	copyComponents(lastIndex, index);
	Entity lastEntity = getEntity(lastIndex);
	getEntity(index) = lastEntity;

	if (mLastChunkCount) [[likely]]
	{
		--mLastChunkCount;
	}
	else
	{
		mChunks.pop_back();
		mLastChunkCount = mChunkCapacity;
	}
	return lastEntity;
}

void Archetype::clear()
{
	assert(mPtrCount == 0 && "There must be no component pointers to this archetype to clear");

	mChunks.clear();
	mLastChunkCount = 0;
}

void Archetype::copyComponents(size_t srcIndex, size_t dstIndex)
{
	for (ComponentId componentId : mComponentIds)
	{
		std::byte* srcData = getData(componentId, srcIndex);
		std::byte* dstData = getData(componentId, dstIndex);
		memcpy(dstData, srcData, ComponentManager::getSize(componentId));
	}
}

void Archetype::copyComponentsTo(Archetype& newArchetype, size_t srcIndex, size_t dstIndex)
{
	for (size_t i = 0; ComponentId componentId : mComponentIds)
	{
		if (componentId == newArchetype.mComponentIds[i])
		{
			std::byte* srcData = getData(componentId, srcIndex);
			std::byte* dstData = newArchetype.getData(componentId, dstIndex);
			memcpy(dstData, srcData, ComponentManager::getSize(componentId));
		}
		else
		{
			do
			{
				++i;
				if (i == newArchetype.mComponentIds.size())
					return;
			} while (componentId != newArchetype.mComponentIds[i]);
		}
	}
}

std::byte* Archetype::getData(ComponentId id, size_t index)
{
	Chunk& chunk = *mChunks[index / mChunkCapacity];
	size_t componentIndex = index % mChunkCapacity;
	return chunk.getData(mComponentOffsets[id]) + componentIndex * ComponentManager::getSize(id);
}

Entity& Archetype::getEntity(size_t index)
{
	Chunk& chunk = *mChunks[index / mChunkCapacity];
	size_t entityIndex = index % mChunkCapacity;
	return chunk.getEntity(entityIndex);
}
