#include "composterData.h"
#include "nbt/nbtSerializer.h"

void composterData::serializeMembers(nbtSerializer& s)
{
	s.serializeMembers(L"compostLevel", fillLevel); 
}
