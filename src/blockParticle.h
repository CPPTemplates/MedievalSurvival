#pragma once
#include "itemID.h"
#include "rectParticle.h"
#include "block.h"
struct blockParticle : rectParticle
{
	blockID brushBlockID;
	blockParticle(const particleID& particleType, const blockID& brushBlockID) :
		rectParticle(particleType, getParticleBrushRect(*blockList[brushBlockID]->tex)), brushBlockID(brushBlockID) {
	}
	blockParticle(const particleID& particleType) :
		rectParticle(particleType), brushBlockID(blockID::air) {
	}
	virtual void serializeValue(nbtSerializer& s) override;
	virtual const resolutionTexture& getTexture() const final;
};