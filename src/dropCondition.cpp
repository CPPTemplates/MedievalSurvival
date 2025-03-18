#include "dropCondition.h"
#include "explosionDrop.h"
#include "blockStateCondition.h"
#include "chanceCondition.h"
#include "tableBonusCondition.h"
#include "enchantmentData.h"
#include "anyOfDropCondition.h"
#include "invertedCondition.h"
#include "toolDropCondition.h"
#include "itemData.h"
#include "filesystem/jsonReader.h"
dropCondition* readCondition(const jsonContainer& container)
{
	const std::wstring conditionName = container[L"condition"].children[0].value;
	dropCondition* conditionToReturn = nullptr;
	if (conditionName == std::wstring(L"minecraft:survives_explosion"))
	{
		conditionToReturn = new explosionDrop();
	}
	else if (conditionName == std::wstring(L"minecraft:block_state_property"))
	{
		blockStateCondition* condition = new blockStateCondition();

		const jsonContainer& conditionContainer = container[L"properties"];

		for (const jsonContainer& blockStateContainer : conditionContainer.children)
		{
			condition->blockStateNames.push_back(blockStateContainer.value);
			condition->blockStateValues.push_back(blockStateContainer.children[0].value);
		}

		conditionToReturn = condition;
	}
	else if (conditionName == std::wstring(L"minecraft:random_chance"))
	{
		chanceCondition* condition = new chanceCondition();
		convertToDouble(container[L"chance"].children[0].value, condition->chance);
		conditionToReturn = condition;
	}
	else if (conditionName == std::wstring(L"minecraft:table_bonus"))
	{
		tableBonusCondition* bonusCondition = new tableBonusCondition();
		const std::vector<jsonContainer> chanceTableContainers = container[L"chances"].children;
		bonusCondition->chanceTable.resize(chanceTableContainers.size());

		const enchantmentID& requiredEnchantmentID = enchantmentDataList.getIDByName(container[L"enchantment"].children[0].value);

		bonusCondition->enchantmentRequired = requiredEnchantmentID;
		for (size_t i = 0; i < chanceTableContainers.size(); i++)
		{
			convertToDouble(chanceTableContainers[i].value, bonusCondition->chanceTable[i]);
		}
		conditionToReturn = bonusCondition;
	}
	else if (conditionName == std::wstring(L"minecraft:any_of"))
	{
		anyOfDropCondition* currentAlternativeDrop = new anyOfDropCondition();
		const std::vector<jsonContainer> termsContainer = container[L"terms"].children;
		for (const jsonContainer& termContainer : termsContainer)
		{
			currentAlternativeDrop->conditionsToCheck.push_back(readCondition(termContainer));
		}
		conditionToReturn = currentAlternativeDrop;
	}
	else if (conditionName == std::wstring(L"minecraft:inverted"))
	{
		invertedCondition* currentInvertedCondition = new invertedCondition();

		currentInvertedCondition->conditionToInvert = readCondition(container[L"term"]);
		conditionToReturn = currentInvertedCondition;
	}
	else if (conditionName == std::wstring(L"minecraft:match_tool"))
	{
		toolDropCondition* currentToolDrop = new toolDropCondition();

		const jsonContainer& predicateContainer = container[L"predicate"];
		if (const jsonContainer* predicatesContainer = predicateContainer.getChild(L"predicates")) {

			if (auto enchantmentsContainer = predicatesContainer->getChild(L"minecraft:enchantments"))
			{
				const std::vector<jsonContainer> enchantmentContainers = enchantmentsContainer->children;
				for (const jsonContainer& enchantmentContainer : enchantmentContainers)
				{
					const std::wstring enchantmentName = enchantmentContainer[L"enchantments"].children[0].value;
					const std::wstring requiredLevelString = enchantmentContainer[L"levels"][L"min"].children[0].value;

					const enchantmentID& enchantmentType = enchantmentDataList.getIDByName(enchantmentName);
					int requiredLevel;
					if (!convertToInt(requiredLevelString, requiredLevel))
					{
						requiredLevel = 1;
					}

					currentToolDrop->requiredEnchantments.push_back(enchantment(enchantmentType, requiredLevel));
				}
			}

		}
		if (auto itemContainer = predicateContainer.getChild(L"items"))
		{
			const std::wstring requiredItemName = itemContainer->children[0].value;
			const itemID& requiredItemID = itemList.getIDByName(requiredItemName);
			if ((int)requiredItemID != -1)
			{
				currentToolDrop->requiredTool = requiredItemID;
			}
		}

		conditionToReturn = currentToolDrop;
	}
	if (conditionToReturn)
	{
		return conditionToReturn;
	}
	else
	{
		return new dropCondition();
		//handleError(std::wstring(L"no condition found");
	}
}
bool dropCondition::checkCondition(const dropData& data, std::mt19937& randomToUse) const
{
	return true;
}