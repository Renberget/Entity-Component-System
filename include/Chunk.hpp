#pragma once
#include "Component.hpp"
#include "Entity.hpp"
#include "Uncopyable.hpp"
#include <new>
#include <array>

class Chunk : public Uncopyable
{
public:
	//Cache line alignment
	constexpr static size_t Alignment = std::hardware_destructive_interference_size;
	constexpr static size_t SizeBits = 14;
	constexpr static size_t Size = 1 << SizeBits;

	Entity& getEntity(size_t index)
	{
		return reinterpret_cast<Entity*>(mData.data())[index];
	}

	template<component T>
	T& getComponent(size_t offset, size_t index)
	{
		return reinterpret_cast<T*>(&mData[offset])[index];
	}

	std::byte* getData(size_t offset)
	{
		return &mData[offset];
	}

private:
	std::array<std::byte, Size> mData;
};
