#include "stdafx.h"
#include "LinkableBlockData.h"
#include "Linkable.h"
#include "nbt/serializeUUID.h"
#include <math/random/random.h>

LinkableBlockData::LinkableBlockData()
{
	identifier = randomUUID(currentRandom);
}
void LinkableBlockData::addToWorld(tickableBlockContainer* containerIn, cveci2& position)
{
	registerLinkable(identifier, this);
	this->containerIn = containerIn;
	this->position = position;
}

void LinkableBlockData::removeFromWorld(tickableBlockContainer* containerIn, cveci2& position)
{
	unRegisterLinkable(identifier);
}

void LinkableBlockData::serializeMembers(nbtSerializer& s)
{
	serializeNBTValue(s, L"uuid", identifier);
}

LinkableBlockData::~LinkableBlockData()
{
	unRegisterLinkable(identifier);
}
