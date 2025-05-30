#include "openData.h"
#include "nbt/nbtSerializer.h"
void openData::serializeMembers(nbtSerializer& s)
{
	blockData::serializeMembers(s);
	s.serializeMembers(std::wstring(L"is open"), isOpen);
}