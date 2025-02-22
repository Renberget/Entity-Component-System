#pragma once
#include "Component.hpp"

template<component T> requires(!std::is_empty_v<T>)
class CompPtr
{
public:
	using CompType = T;

#ifdef NDEBUG
	explicit CompPtr(T* ptr) :
		mPtr(ptr)
	{}
#else
	explicit CompPtr(T* ptr, uint32_t& ptrCount) :
		mPtr(ptr),
		mPtrCount(ptrCount)
	{
		++mPtrCount;
	}

	~CompPtr()
	{
		--mPtrCount;
	}

	CompPtr(const CompPtr& other) :
		mPtr(other.mPtr),
		mPtrCount(other.mPtrCount)
	{
		++mPtrCount;
	}

	CompPtr& operator=(const CompPtr& other)
	{
		mPtr = other.mPtr;
		mPtrCount = other.mPtrCount;
		++mPtrCount;
	}
#endif

	void operator=(const T& component)
	{
		*mPtr = component;
	}

	T operator*() const
	{
		return *mPtr;
	}

	T* operator->()
	{
		return mPtr;
	}

	const T* operator->() const
	{
		return mPtr;
	}

protected:
	T* mPtr;

private:
#ifndef NDEBUG
	uint32_t& mPtrCount;
#endif
};
