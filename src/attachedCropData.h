#pragma once
#include "attachedBlockData.h"
#include "cropData.h"
struct attachedCropData : cropData, attachedBlockData
{
	attachedCropData(cint& age = 0) : cropData(age), attachedBlockData() {}
	virtual void serializeMembers(nbtSerializer& s) override {
		return cropData::serializeMembers(s);
	}
	virtual bool tick(tickableBlockContainer* containerIn, cveci2& position) override {
		return attachedBlockData::tick(containerIn, position);
	}
	virtual void randomTick(tickableBlockContainer* containerIn, cveci2& position) override {
		return cropData::randomTick(containerIn, position);
	}
};