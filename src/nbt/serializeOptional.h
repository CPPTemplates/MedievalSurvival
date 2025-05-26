#pragma once
#include "nbt/nbtSerializer.h"
template <typename T>
inline bool serializeNBTValue(nbtSerializer& s, const std::wstring& memberName, std::optional<T>& value)
{
	if (s.write) {
		if (value.has_value())
		{
			return serializeNBTValue(s, memberName, *value);
		}
	}
	else {
		T temporary = T();
		if (serializeNBTValue(s, memberName, temporary)) {
			value = temporary;
			return true;
		}
		else {
			value = std::nullopt;
		}
	}
	return false;
}