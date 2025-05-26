#include "stdafx.h"
#include "HitboxSound.h"
#include <soundCollection.h>
#include "dimension.h"

void HitboxSound::update(entity* e)
{
	soundCollection::updateSound(e->dimensionIn, UpdateSoundPacket(id, e->speed, e->position + relativeLocation));
}
