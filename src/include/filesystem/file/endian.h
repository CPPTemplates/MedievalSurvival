#pragma once
#include <cstdint>
#include <utility>
#include <bit>

// no pointers, const values and non-fundamental types accepted
template <typename T>
inline constexpr bool is_endian_convertable_v = (!(std::is_pointer_v<T> || std::is_const_v<T> || std::is_same_v<T, void>)&&std::is_fundamental_v<T>) || std::is_enum_v<T>;

inline void invertEndian(byte *ptr, cint &size)
{
	byte *endPtr = ptr + size - 1;
	while (endPtr > ptr)
	{
		std::swap(*ptr++, *endPtr--);
	}
}

template <typename T, typename = std::enable_if_t<is_endian_convertable_v<T>>>
inline void invertEndian(T &value)
{
	if constexpr (sizeof(T) > 1)
	{
		invertEndian((byte *)&value, sizeof(value));
	}
}
