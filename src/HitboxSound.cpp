#include "stdafx.h"
#include "HitboxSound.h"
#include <audioCollection.h>
#include "dimension.h"

void HitboxSound::update(entity* e)
{
	audioCollection::updateSound(e->dimensionIn, UpdateSoundPacket(id, e->speed, e->position + relativeLocation));
}
