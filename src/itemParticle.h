#pragma once
#include "itemID.h"
#include "rectParticle.h"
#include "itemData.h"

struct itemParticle : rectParticle
{
	itemID brushItemID;
	itemParticle(const particleID& particleType, const blockID& brushItemID = itemID()) :
		rectParticle(particleType, getParticleBrushRect(*itemList[brushItemID]->tex)), brushItemID(brushItemID) {
	}
	virtual void serializeValue(nbtSerializer& s) override;
	virtual const resolutionTexture& getTexture() const final;
};