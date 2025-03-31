#pragma once
#include "fourLeggedMob.h"
#include "passiveMob.h"
struct fourLeggedPassiveMob : public fourLeggedMob, public passiveMob
{
	fourLeggedPassiveMob(const entityID& entityType) : mob(entityType), fourLeggedMob(entityType), passiveMob(entityType) {}

	virtual void updateBodyParts() const override {
		fourLeggedMob::updateBodyParts();
	}
};