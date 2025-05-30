#pragma once
#include "entity.h"
struct movableBlockContainer;
struct piston : public entity
{
	veci2 connectedPistonPosition = cveci2();
	movableBlockContainer* movingBlocks = nullptr;

	piston();
	virtual void tick() override;
	virtual void render(const gameRenderData& targetData) const override;

	virtual ~piston() override;
	virtual fp getGravityForce() const override;
	virtual void serializeMembers(nbtSerializer& s) override;
};