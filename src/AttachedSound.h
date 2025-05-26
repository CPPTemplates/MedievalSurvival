#pragma once
#include <entity.h>
struct AttachedSound
{
	AttachedSound(const uuid& id, int ticksLeft): id(id), ticksLeft(ticksLeft){}
	uuid id;
	int ticksLeft = 0;

	virtual void update(entity* e);
};