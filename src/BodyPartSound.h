#include "AttachedSound.h"
#include "math/bodypart2d.h"
#include "math/uuid.h"
#pragma once
struct BodyPartSound :AttachedSound
{
	bodyPart2D* bodyPart;
	//the location of the sound on the body part of the entity
	vec2 relativeLocation;
	BodyPartSound(uuid id, int ticksLeft, bodyPart2D* bodyPart, vec2 relativeLocation = vec2()): AttachedSound(id, ticksLeft), bodyPart(bodyPart), relativeLocation(relativeLocation){}
	virtual void update(entity* e) override;
};