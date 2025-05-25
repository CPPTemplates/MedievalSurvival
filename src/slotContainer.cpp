#include "slotContainer.h"
#include "world.h"
#include "include/optimization/handleError.h"
bool slotContainer::addToEqualStacks(itemStack& stack, itemStack*& emptySlot)
{
	handleError(std::wstring(L"not implemented"));
	return false;
}
bool slotContainer::addStack(itemStack& stack)
{
	itemStack* emptySlot = nullptr;
	return addToEqualStacks(stack, emptySlot) || (emptySlot && emptySlot->addStack(stack));
}

bool slotContainer::substractStack(itemStack& stack)
{
	currentWorld->currentChat.addLine(std::wstring(L"substraction not implemented"));
	return false;
}

void slotContainer::serializeValue(nbtSerializer& s)
{
	handleError(std::wstring(L"not implemented"));
}

void slotContainer::clearData()
{
}