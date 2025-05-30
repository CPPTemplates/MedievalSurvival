#pragma once
#include"blockData.h"
struct composterData : blockData
{
	fp fillLevel = 0;
	virtual void serializeMembers(nbtSerializer& s) override;
};