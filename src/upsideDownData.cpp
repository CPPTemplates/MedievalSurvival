#include "upsideDownData.h"
#include "nbt/nbtSerializer.h"
void upsideDownData::serializeMembers(nbtSerializer& s)
{
	serializeNBTValue(s, std::wstring(L"upside down"), upsideDown);
}