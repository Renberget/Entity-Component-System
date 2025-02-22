#pragma once
#include "Chunk.hpp"
#include <memory>
#include <vector>

using ChunkPtr = std::unique_ptr<Chunk, void(*)(Chunk*)>;

class ChunkAllocator
{
	constexpr static size_t PreallocatedChunks = 8;

public:
	static ChunkPtr allocate();

private:
	ChunkAllocator();
	~ChunkAllocator();
	static void free(Chunk* chunk);
	static void fill();

	static ChunkAllocator sInstance;
	std::vector<Chunk*> mFreeChunks;
};