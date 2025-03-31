#pragma once
#include "entity.h"
struct fallingBlock : public entity
{
	blockID linkedBlockID;
	fallingBlock(const blockID& linkedBlockID = blockID::sand);
	virtual void render(const gameRenderData& targetData) const override;
	virtual void tick() override;
	virtual void serializeValue(nbtSerializer& s) override;
};