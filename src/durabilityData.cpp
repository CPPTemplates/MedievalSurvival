#include "durabilityData.h"
#include "nbt/nbtSerializer.h"
void durabilityData::serializeMembers(nbtSerializer& s)
{
	serializeNBTValue(s, std::wstring(L"durability"), durability);
}