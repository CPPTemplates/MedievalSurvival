#include "breedableMob.h"
#include "nbt/nbtSerializer.h"
void breedableMob::tick()
{
	age++;
}

void breedableMob::serializeMembers(nbtSerializer& s)
{
	serializeNBTValue(s, std::wstring(L"age"), age);
}