#pragma once
#include"blockData.h"
struct composterData : blockData
{
	fp fillLevel = 0;
	virtual void serializeValue(nbtSerializer& s) override;
};