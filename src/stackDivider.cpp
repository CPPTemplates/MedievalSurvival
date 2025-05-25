
#include "stackDivider.h"
#include "itemData.h"
#include "array/arrayFunctions/sortedArray.h"

bool stackDivider::addStack(itemStack& s) {
	if (!stackHolding.addStack(s))return false;
	originalStack = stackHolding.clone<itemStack>();//copy stack
	return true;
}

bool stackDivider::interactWith(itemStack* candicate, cmb& button) {
	if (originalStack.count
		//candidate stack is empty or has the same items as the original stack
		&& (!candicate->count || candicate->compare(originalStack))) {
		cint& maxStackSize = itemList[originalStack.stackItemID]->maxStackSize;
		auto getRoom = [button, maxStackSize](itemStack* stack) {
			return (button == mb::Left) ?
				maxStackSize - stack->count :
				//true converted to 1, false to 0
				maxStackSize > stack->count;
			};
		cint& room = getRoom(candicate);
		if (room <= 0) {
			return false;
		}

		//how much to put in here at most
		std::vector<int> roomPerStack = std::vector<int>(divideOver.size());
		//divide evenly
		for (size_t i = 0; i < divideOver.size(); i++) {
			//check if the stack still has the same item type stored in it
			if (divideOver[i]->compare(originalStack)) {
				//retrieve stacks (add them back up to stackholding)
				stackHolding.addStack(*divideOver[i], (int)amountsDivided[i]);
				roomPerStack[i] = getRoom(divideOver[i]);
			}
		}
		roomPerStack.push_back(room);
		divideOver.push_back(candicate);

		int total = stackHolding.count;

		std::vector<int> newAmounts = divideOverArray(total, roomPerStack);
		for (size_t i = 0; i < divideOver.size(); i++) {
			divideOver[i]->addStack(stackHolding, (int)newAmounts[i]);
		}
		amountsDivided = newAmounts;
	}
	else {
		if (candicate->count) {
			if (divideOver.size()) {
				return false;
			}
		}
		//swap slots (also works if occupying slot is empty)
		std::swap(*candicate, stackHolding);
		originalStack = stackHolding;
		pickingUp = true;
	}

	return true;
}
