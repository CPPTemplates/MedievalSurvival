#pragma once
#include "itemTag.h"
struct bucketData :itemTag
{
	fluidLevel fillLevel = 0;
	bucketData(fluidLevel fillLevel = maxFluidLevel) :fillLevel(fillLevel) {}
	virtual void serializeMembers(nbtSerializer& s) override;
};