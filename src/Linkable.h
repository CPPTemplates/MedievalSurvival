#include "include/math/uuid.h"
#include <set>
#include "nbt/nbtSerializable.h"
#pragma once
extern std::unordered_map<uuid, void*> links;
inline void registerLinkable(const uuid& identifier, void* pointer) {
	links.insert({ identifier, pointer });
}

inline void unRegisterLinkable(const uuid& identifier)
{
	links.erase(identifier);
}

inline void* getLinkable(const uuid& identifier)
{
	const auto& result = links.find(identifier);
	return result == links.end() ? nullptr : result->second;
}
