#pragma once
#include "pollenData.h"
#include "doubleAttachedBlockData.h"
struct doubleAttachedBlockPollenData : pollenData, doubleAttachedBlockData
{
	doubleAttachedBlockPollenData(cbool& isPart0 = false) : pollenData(), doubleAttachedBlockData(isPart0) {}

	virtual void serializeMembers(nbtSerializer& s) override {
		return doubleAttachedBlockData::serializeMembers(s);
	}
	virtual void randomTick(tickableBlockContainer* containerIn, cveci2& position) override {
		return pollenData::randomTick(containerIn, position);
	}
	virtual bool tick(tickableBlockContainer* containerIn, cveci2& position) override {
		return doubleAttachedBlockData::tick(containerIn, position);
	}
};