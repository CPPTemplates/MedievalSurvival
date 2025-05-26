#include "stdafx.h"
#include "UpdateSoundPacket.h"
#include "nbt/serializeVector.h"
#include "nbt/serializeOptional.h"

bool UpdateSoundPacket::serialize(nbtSerializer& s)
{
    serializeNBTValue(s, L"newPosition", newPosition);
    serializeNBTValue(s, L"newVelocity", newVelocity);
    serializeNBTValue(s, L"newPitch", newPitch);
    serializeNBTValue(s, L"newVolume", newVolume);
    return SoundPacket::serialize(s);
}
