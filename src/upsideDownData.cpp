#include "upsideDownData.h"
#include "nbt/nbtSerializer.h"
void upsideDownData::serializeMembers(nbtSerializer& s)
{
	s.serializeMembers(std::wstring(L"upside down"), upsideDown);
}