#include "anyOfDropCondition.h"
bool anyOfDropCondition::checkCondition(const dropData& data, std::mt19937& randomToUse) const
{
	for (dropCondition* conditionToCheck : conditionsToCheck)
	{
		if (conditionToCheck->checkCondition(data, randomToUse))
		{
			return true;
		}
	}
	return false;
}

anyOfDropCondition::~anyOfDropCondition()
{
	for (dropCondition* const& condition : conditionsToCheck)
	{
		delete condition;
	}
}

