#include "ChunkAllocator.hpp"

ChunkAllocator ChunkAllocator::sInstance;

ChunkPtr ChunkAllocator::allocate()
{
	Chunk* chunk = sInstance.mFreeChunks.back();
	sInstance.mFreeChunks.pop_back();
	fill();
	return ChunkPtr(chunk, &ChunkAllocator::free);
}

ChunkAllocator::ChunkAllocator()
{
	fill();
}

ChunkAllocator::~ChunkAllocator()
{
	for (Chunk* chunk : mFreeChunks)
		_aligned_free(static_cast<void*>(chunk));
}

void ChunkAllocator::free(Chunk* chunk)
{
	sInstance.mFreeChunks.emplace_back(chunk);
}

void ChunkAllocator::fill()
{
	while (sInstance.mFreeChunks.size() < PreallocatedChunks)
		sInstance.mFreeChunks.emplace_back(static_cast<Chunk*>(_aligned_malloc(sizeof(Chunk), Chunk::Alignment)));
}