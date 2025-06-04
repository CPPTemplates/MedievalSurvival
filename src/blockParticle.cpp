#include "block.h"
#include "blockParticle.h"
#include "nbt/nbtSerializer.h"

void blockParticle::serializeMembers(nbtSerializer& s)
{
	rectParticle::serializeMembers(s);
	serializeNBTValue(s, std::wstring(L"brush block id"), brushBlockID);
}

const resolutionTexture& blockParticle::getTexture() const
{
	return *blockList[brushBlockID]->tex;
}