#pragma once
#include "throwable.h"
#include "math/bodypart2d.h"
struct arrow : public throwable
{
	arrow();
	virtual void render(const gameRenderData& targetData) const override;
	virtual void onCollision(const std::vector<entity*>& collidingEntities) override;
	bodyPart2D* mainBodyPart = nullptr;
	fp impactAngle = 0;
};