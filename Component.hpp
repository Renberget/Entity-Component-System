#pragma once
#include "Entity.hpp"
#include <type_traits>

template<typename T>
concept component = std::is_trivial_v<T>
&& !std::is_const_v<T>
&& !std::is_array_v<T>
&& !std::is_reference_v<T>
&& !std::is_pointer_v<T>
&& !std::same_as<std::remove_pointer_t<std::decay_t<T>>, Entity>;

struct Prefab {};
