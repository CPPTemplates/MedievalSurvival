#include "nbt/nbtSerializer.h"
#include "bambooData.h"
void bambooData::serializeMembers(nbtSerializer& s)
{
	ageData::serializeMembers(s);
	s.serializeMembers(std::wstring(L"leaves size"), leavesSize);
	s.serializeMembers(std::wstring(L"stalk thickness"), stalkThickness);
}