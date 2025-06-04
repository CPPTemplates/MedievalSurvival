#include "endPortalFrameData.h"
#include "nbt/nbtSerializer.h"
void endPortalFrameData::serializeMembers(nbtSerializer& s)
{
	blockData::serializeMembers(s);
	facingData::serializeMembers(s);
	serializeNBTValue(s, std::wstring(L"has eye"), hasEye);
}