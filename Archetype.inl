#include "Archetype.hpp"

template<component T>
CompPtr<T> Archetype::getComponent(size_t index)
{
#ifdef NDEBUG
	return CompPtr<T>(reinterpret_cast<T*>(getData(ComponentManager::getId<T>(), index)));
#else
	return CompPtr<T>(reinterpret_cast<T*>(getData(ComponentManager::getId<T>(), index)), mPtrCount);
#endif
}

template<typename F>
void Archetype::forEach(F&& callback)
{
	using function_type = decltype(std::function{ std::forward<F>(callback) });
	forEach(std::forward<F>(callback), function_type{});
}

template<typename F, typename R, typename... Args>
void Archetype::forEach(F&& callback, const std::function<R(Args...)>&)
{
	for (size_t chunkIndex = 0; chunkIndex < mChunks.size(); ++chunkIndex)
	{
		for (size_t i = 0; i < ((chunkIndex == mChunks.size() - 1) ? mLastChunkCount : mChunkCapacity); ++i)
		{
			std::invoke(std::forward<F>(callback), std::forward<Args>(getData<Args>(*mChunks[chunkIndex], i))...);
		}
	}
}

template<typename T>
T Archetype::getData(Chunk& chunk, size_t index)
{
	if constexpr (std::same_as<T, Entity>)
		return chunk.getEntity(index);

	else
	{
		using C = T::CompType;

		if constexpr (std::same_as<T, CompPtr<C>>)
		{
			assert(mMask.has<C>() && "The component must be present in the archetype");
#ifdef NDEBUG
			return CompPtr<C>(&chunk.getComponent<C>(mComponentOffsets[ComponentManager::getId<C>()], index));
#else
			return CompPtr<C>(&chunk.getComponent<C>(mComponentOffsets[ComponentManager::getId<C>()], index), mPtrCount);
#endif
		}

		else if constexpr (std::same_as<T, CompOpt<C>>)
		{
#ifdef NDEBUG
			return CompOpt<C>(mMask.has<C> ? &chunk.getComponent<C>(mComponentOffsets[ComponentManager::getId<C>()], index) : nullptr);
#else
			return CompOpt<C>(mMask.has<C> ? &chunk.getComponent<C>(mComponentOffsets[ComponentManager::getId<C>()], index) : nullptr, mPtrCount);
#endif
		}

		else
			static_assert(false, "Parameter should be Entity, CompPtr<T> or CompOpt<T>");
	}

	return *reinterpret_cast<T*>(nullptr);
}
