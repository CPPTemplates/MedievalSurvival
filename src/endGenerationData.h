#pragma once
#include "generationData.h"
#include <vector>
#include "math/vector/vectn.h"
struct endGenerationData :generationData
{
	endGenerationData() {}
	virtual void serializeMembers(nbtSerializer& s) override;
	virtual ~endGenerationData() override;
	std::vector<veci2> groundPositions = std::vector<veci2>();
};