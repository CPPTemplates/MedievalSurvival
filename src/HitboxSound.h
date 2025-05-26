#include "AttachedSound.h"
#pragma once
struct HitboxSound : AttachedSound
{
	//the location of the sound on the hitbox of the entity
	vec2 relativeLocation;
	virtual void update(entity* e) override;
};