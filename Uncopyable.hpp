#pragma once

class Uncopyable
{
public:
	Uncopyable() = default;
	Uncopyable(const Uncopyable&) = delete;
	Uncopyable& operator=(const Uncopyable&) = delete;
	Uncopyable(Uncopyable&&) noexcept = default;
	Uncopyable& operator=(Uncopyable&&) noexcept = default;
};
