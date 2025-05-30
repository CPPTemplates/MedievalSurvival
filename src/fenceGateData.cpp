#include "fenceGateData.h"
void fenceGateData::serializeMembers(nbtSerializer& s)
{
	blockData::serializeMembers(s);
	facingData::serializeMembers(s);
	openData::serializeMembers(s);
}