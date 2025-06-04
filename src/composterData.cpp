#include "composterData.h"
#include "nbt/nbtSerializer.h"

void composterData::serializeMembers(nbtSerializer& s)
{
	serializeNBTValue(s, L"compostLevel", fillLevel); 
}
