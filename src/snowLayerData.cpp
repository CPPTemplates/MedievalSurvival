#include "snowLayerData.h"
#include "nbt/nbtSerializer.h"

snowLayerData::snowLayerData(cfp& layerThickness)
{
	this->layerThickness = layerThickness;
}

void snowLayerData::serializeMembers(nbtSerializer& s)
{
	blockData::serializeMembers(s);
	fallingBlockData::serializeMembers(s);
	s.serializeMembers(std::wstring(L"layer thickness"), layerThickness);
}
