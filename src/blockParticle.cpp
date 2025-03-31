#include "block.h"
#include "blockParticle.h"
#include "nbt/nbtSerializer.h"

void blockParticle::serializeValue(nbtSerializer& s)
{
	rectParticle::serializeValue(s);
	s.serializeValue(std::wstring(L"brush block id"), brushBlockID);
}

const resolutionTexture& blockParticle::getTexture() const
{
	return *blockList[brushBlockID]->tex;
}