#pragma once
#include "blockData.h"
struct attachedData : virtual blockData
{
	bool attached = false;
	attachedData(cbool& attached = false) :attached(attached) {}
	virtual void serializeMembers(nbtSerializer& s) override;
};