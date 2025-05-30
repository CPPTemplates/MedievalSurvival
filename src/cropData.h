#pragma once
#include "ageData.h"
struct cropData : virtual ageData
{
	cropData(cint& age = 0) : ageData(age) {}
	virtual void randomTick(tickableBlockContainer* containerIn, cveci2& position) override;
	virtual void serializeMembers(nbtSerializer& s) override;
};