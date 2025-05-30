#include "attachedData.h"
#include "nbt/nbtSerializer.h"
void attachedData::serializeMembers(nbtSerializer& s)
{
	s.serializeMembers(std::wstring(L"attached"), attached);
}