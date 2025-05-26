#include "stdafx.h"
#include "BodyPartSound.h"
#include "soundCollection.h"
#include "dimension.h"

void BodyPartSound::update(entity* e)
{
	soundCollection::updateSound(e->dimensionIn, UpdateSoundPacket(id, e->speed, bodyPart->getCumulativeParentTransform().multPointMatrix(relativeLocation)));
}
