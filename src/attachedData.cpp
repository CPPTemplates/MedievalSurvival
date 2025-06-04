#include "attachedData.h"
#include "nbt/nbtSerializer.h"
void attachedData::serializeMembers(nbtSerializer& s)
{
	serializeNBTValue(s, std::wstring(L"attached"), attached);
}