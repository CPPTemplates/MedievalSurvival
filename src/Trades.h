#pragma once
#include "itemStack.h"
#include "include/filesystem/jsonReader.h"
void readTrades();
struct Trade: nbtSerializable {
	itemStack wanted[2]{};
	itemStack given = itemStack();
	fp multiplier = 0.02;
	int maxItemsInstock = 12;
	int itemsInStock = 12;
	virtual void serializeMembers(nbtSerializer& s) override;
};