#pragma once
#include "overWorldBiomeGenerator.h"
struct mushroomFields : public overWorldBiomeGenerator
{
	mushroomFields();
	virtual void  attemptgenerateStructures(dimension* dimensionIn, cveci2& pos, std::mt19937& randomToUse) const override;
};