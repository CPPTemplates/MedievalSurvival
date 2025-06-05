#pragma once
#include "itemTag.h"
//we might as well include it; most of the blockdata files use the vector
#include "math/vector/vectn.h"
struct blockData : itemTag
{
	bool ticked = false;
	blockData() {}
	//returns if it still needs to be ticked
	virtual bool tick(struct tickableBlockContainer* containerIn, cveci2& position);
	virtual void randomTick(struct tickableBlockContainer* containerIn, cveci2& position);
	//called when the block is for example destroyed by a tnt or broken by a player
	virtual void onBlockRemove(struct tickableBlockContainer* containerIn, cveci2& position);
	virtual void serializeMembers(nbtSerializer& s) override;
	//called when a block is given a place in a container
	virtual void addToWorld(struct tickableBlockContainer* containerIn, cveci2& position);
	//called when a block is removed from a container or when a container invalidates
	virtual void removeFromWorld(struct tickableBlockContainer* containerIn, cveci2& position);
};
blockData* createBlockData(const blockID& block);