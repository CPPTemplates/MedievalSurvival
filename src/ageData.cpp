#include "ageData.h"
#include "nbt/nbtSerializer.h"
void ageData::serializeMembers(nbtSerializer& s)
{
	s.serializeMembers(std::wstring(L"age"), age);
}