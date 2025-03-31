#include "itemParticle.h"
#include "itemData.h"
#include "nbt/nbtSerializer.h"

const resolutionTexture& itemParticle::getTexture() const
{
	return *itemList[brushItemID]->tex;
}

void itemParticle::serializeValue(nbtSerializer& s)
{
	s.serializeValue(std::wstring(L"brush item id"), brushItemID);
	rectParticle::serializeValue(s);
}