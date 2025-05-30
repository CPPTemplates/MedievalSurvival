#include "stairsData.h"
void stairsData::serializeMembers(nbtSerializer& s)
{
	facingData::serializeMembers(s);
	upsideDownData::serializeMembers(s);
}