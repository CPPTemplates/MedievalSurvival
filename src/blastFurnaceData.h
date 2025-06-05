#pragma once
#include "furnaceData.h"
#include "LinkableBlockData.h"
struct blastFurnaceData : public virtual furnaceData, LinkableBlockData {
	using furnaceData::tick;
	virtual void serializeMembers(nbtSerializer& s) override;
};