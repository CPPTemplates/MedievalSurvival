#pragma once
#include "entity.h"
struct tnt : public entity
{
	tnt();
	int ticksUntilFuse = 0;
	virtual void tick() override;
	virtual void render(const gameRenderData& targetData) const override;
	virtual void serializeValue(nbtSerializer& s) override;
};