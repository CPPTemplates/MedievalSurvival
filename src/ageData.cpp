#include "ageData.h"
#include "nbt/nbtSerializer.h"
void ageData::serializeMembers(nbtSerializer& s)
{
	serializeNBTValue(s, std::wstring(L"age"), age);
}