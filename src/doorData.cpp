#include "doorData.h"
void doorData::serializeMembers(nbtSerializer& s)
{
	blockData::serializeMembers(s);
	doubleBlockData::serializeMembers(s);
	facingData::serializeMembers(s);
	openData::serializeMembers(s);
}