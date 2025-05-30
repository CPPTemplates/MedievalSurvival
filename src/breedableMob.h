#pragma once
#include "mob.h"
struct breedableMob : virtual mob
{
	breedableMob(const entityID& entityType) :
		mob(entityType) {}
	int age = 0;
	virtual void tick() override;
	virtual void serializeMembers(nbtSerializer& s) override;
};