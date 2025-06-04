#pragma once
#include <itemStack.h>
#include <filesystem/jsonReader.h>
#include <itemData.h>

inline bool readItemStack(const jsonContainer& container, itemStack& stack) {

	const std::wstring resultItemName = container[L"id"].children[0].value;

	const itemID& resultItemIndex = itemList.getIDByName(resultItemName);
	if ((int)resultItemIndex == -1)
	{
		return false;
	}
	stack.stackItemID = resultItemIndex;

	stack.data = createItemTag(stack.stackItemID);
	if (const jsonContainer* countContainer = container.getChild(L"count"))
	{
		if (!convertToInt(countContainer->children[0].value, stack.count)) {
			//we didn't convert the amount, but for the rest we still have a valid item stack
			return false;
		}
	}
	else
	{
		stack.count = 1;
	}
	return true;
}