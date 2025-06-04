#include "stdafx.h"
#include "StartSoundPacket.h"
#include "nbt/serializeVector.h"
#include "math/random/random.h"
#include "nbt/serializeOptional.h"

bool StartSoundPacket::serialize(nbtSerializer& s)
{
	serializeNBTValue(s, L"position", position);
	serializeNBTValue(s, L"key", key);
	serializeNBTValue(s, L"sound index", soundIndex);
	serializeNBTValue(s, L"volume", volume);
	serializeNBTValue(s, L"pitch", pitch);
	serializeNBTValue(s, L"shouldLoop", shouldLoop);
	return SoundPacket::serialize(s);
}
