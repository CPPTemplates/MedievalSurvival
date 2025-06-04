#include "fireworkData.h"
#include "nbt/nbtSerializer.h"
#include "nbt/serializeList.h"
void fireWorkData::serializeMembers(nbtSerializer& s)
{
	serializeNBTValue(s, std::wstring(L"flight duration"), flightDuration);
	serializeNBTValue(s, std::wstring(L"firework stars"), (nbtSerializable&)fireworkStars);
}