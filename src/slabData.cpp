#include "slabData.h"
#include "nbt/nbtSerializer.h"
void slabData::serializeMembers(nbtSerializer& s)
{
	s.serializeMembers(std::wstring(L"slab type"), type);
}