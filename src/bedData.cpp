#include "bedData.h"
void bedData::serializeMembers(nbtSerializer& s)
{
	blockData::serializeMembers(s);
	doubleBlockData::serializeMembers(s);
	facingData::serializeMembers(s);
}