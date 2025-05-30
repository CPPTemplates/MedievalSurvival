#include "durabilityData.h"
#include "nbt/nbtSerializer.h"
void durabilityData::serializeMembers(nbtSerializer& s)
{
	s.serializeMembers(std::wstring(L"durability"), durability);
}