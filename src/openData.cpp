#include "openData.h"
#include "nbt/nbtSerializer.h"
void openData::serializeMembers(nbtSerializer& s)
{
	blockData::serializeMembers(s);
	serializeNBTValue(s, std::wstring(L"is open"), isOpen);
}