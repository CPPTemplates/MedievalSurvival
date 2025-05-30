#pragma once
#include "entity.h"
#include "fireworkData.h"
struct fireworkRocket : public entity
{
	int ticksToExist = 0;
	fireworkRocket() : entity(entityID::firework_rocket) {}
	fireWorkData data;
	virtual void serializeMembers(nbtSerializer& s) override;
	virtual void render(const gameRenderData& targetData) const override;
};