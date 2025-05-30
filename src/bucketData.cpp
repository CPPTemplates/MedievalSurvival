#include "bucketData.h"
#include "nbt/nbtSerializer.h"
void bucketData::serializeMembers(nbtSerializer& s)
{
	s.serializeMembers(std::wstring(L"fluid level"), fillLevel);
}