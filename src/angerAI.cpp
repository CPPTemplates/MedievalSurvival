#include "angerAI.h"
#include "nbt/nbtSerializer.h"
bool angerAI::shouldExecute()
{
	return ticksAngry > 0 && targetAI::shouldExecute();
}

void angerAI::serializeMembers(nbtSerializer& s)
{
	targetAI::serializeMembers(s);
	serializeNBTValue(s, std::wstring(L"ticks angry"), ticksAngry);
}