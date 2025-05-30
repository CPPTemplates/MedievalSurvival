#pragma once
#include "itemTag.h"
struct durabilityData : itemTag
{
	fp durability = 1;
	virtual void serializeMembers(nbtSerializer& s) override;
};