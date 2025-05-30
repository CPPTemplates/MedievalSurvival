#include "humanoid.h"
#pragma once
struct zombifiedPiglin : public humanoid
{
	zombifiedPiglin();
	virtual void render(const gameRenderData& targetData) const override;
	virtual void serializeMembers(nbtSerializer& s) override;
};