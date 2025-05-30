#pragma once
#include "interface/idestructable.h"
#include "nbt/nbtSerializable.h"
struct generationData :IDestructable, nbtSerializable
{
	virtual void serializeMembers(nbtSerializer& s) override;
};