#pragma once
#include "blockData.h"
constexpr int maxSapLevel = 0x6;//maximum = 16 blocks
struct sapSpreadData : virtual blockData
{
	int sapLevel;
	sapSpreadData(cint& sapLevel = maxSapLevel) :sapLevel(sapLevel) {}
	virtual void serializeMembers(nbtSerializer& s) override;
	virtual void randomTick(tickableBlockContainer* containerIn, cveci2& position) override;
};