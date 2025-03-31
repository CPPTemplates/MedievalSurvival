#pragma once
#include "mob.h"
struct passiveMob : virtual mob
{
	passiveMob(const entityID& entityType);
};