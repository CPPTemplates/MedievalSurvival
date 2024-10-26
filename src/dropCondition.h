#pragma once
#include <random>
#include "constants.h"
#include "interface/idestructable.h"
struct jsonContainer;
struct dropCondition : IDestructable
{
	virtual bool checkCondition(const dropData& data, std::mt19937& randomToUse) const;
};
dropCondition* readCondition(const struct jsonContainer& container);