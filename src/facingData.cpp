#include "facingData.h"
#include "nbt/nbtSerializer.h"
void facingData::serializeMembers(nbtSerializer& s)
{
	serializeNBTValue(s, std::wstring(L"facing"), directionFacing);
}