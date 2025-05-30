#include "trapDoorData.h"
void trapDoorData::serializeMembers(nbtSerializer& s)
{
	facingData::serializeMembers(s);
	upsideDownData::serializeMembers(s);
	openData::serializeMembers(s);
}