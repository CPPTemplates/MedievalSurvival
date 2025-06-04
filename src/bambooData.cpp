#include "nbt/nbtSerializer.h"
#include "bambooData.h"
void bambooData::serializeMembers(nbtSerializer& s)
{
	ageData::serializeMembers(s);
	serializeNBTValue(s, std::wstring(L"leaves size"), leavesSize);
	serializeNBTValue(s, std::wstring(L"stalk thickness"), stalkThickness);
}