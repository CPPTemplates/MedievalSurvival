#include "composterData.h"
#include "nbt/nbtSerializer.h"

void composterData::serializeValue(nbtSerializer& s)
{
	s.serializeValue(L"compostLevel", fillLevel); 
}
