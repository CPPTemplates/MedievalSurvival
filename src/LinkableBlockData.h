#pragma once
#include "blockData.h"
//makes sure a block can be found back. comes in handy when a villager is looking for his job block but it got moved with a piston.
struct LinkableBlockData : virtual blockData {
	uuid identifier;
	tickableBlockContainer* containerIn = nullptr;
	veci2 position = veci2();
	LinkableBlockData();
	virtual void addToWorld(tickableBlockContainer* containerIn, cveci2& position) override;
	virtual void removeFromWorld(tickableBlockContainer* containerIn, cveci2& position) override;
	virtual void serializeMembers(nbtSerializer& s) override;
	~LinkableBlockData() override;
};