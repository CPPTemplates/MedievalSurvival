#pragma once
#include "entity.h"
struct throwable : public entity
{
	throwable(const entityID& entityType);
	virtual void tick() override;
	virtual void render(const gameRenderData& targetData) const override;
	virtual void onCollision(const std::vector<entity*>& collidingEntities);
	virtual void serializeValue(nbtSerializer& s) override;
	void addImpactDamage(const std::vector<entity*>& collidingEntities, cfp& damage);
	uuid thrownFrom = uuid();
	bool leftOwner = false;
};