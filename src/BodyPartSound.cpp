#include "stdafx.h"
#include "BodyPartSound.h"
#include "audioCollection.h"
#include "dimension.h"

void BodyPartSound::update(entity* e)
{
	audioCollection::updateSound(e->dimensionIn, UpdateSoundPacket(id, e->speed, bodyPart->getCumulativeParentTransform().multPointMatrix(relativeLocation)));
}
