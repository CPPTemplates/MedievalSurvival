#include "slabData.h"
#include "nbt/nbtSerializer.h"
void slabData::serializeMembers(nbtSerializer& s)
{
	serializeNBTValue(s, std::wstring(L"slab type"), type);
}