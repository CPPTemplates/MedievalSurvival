#include "bucketData.h"
#include "nbt/nbtSerializer.h"
void bucketData::serializeMembers(nbtSerializer& s)
{
	serializeNBTValue(s, std::wstring(L"fluid level"), fillLevel);
}