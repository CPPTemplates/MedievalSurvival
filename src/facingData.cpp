#include "facingData.h"
#include "nbt/nbtSerializer.h"
void facingData::serializeMembers(nbtSerializer& s)
{
	s.serializeMembers(std::wstring(L"facing"), directionFacing);
}