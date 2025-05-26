#include "stdafx.h"
#include "StartSoundPacket.h"
#include "nbt/serializeVector.h"
#include "math/random/random.h"
#include "nbt/serializeOptional.h"

bool StartSoundPacket::serialize(nbtSerializer& s)
{
	serializeNBTValue(s, L"position", position);
	s.serializeValue(L"key", key);
	s.serializeValue(L"sound index", soundIndex);
	s.serializeValue(L"volume", volume);
	s.serializeValue(L"pitch", pitch);
	s.serializeValue(L"shouldLoop", shouldLoop);
	return SoundPacket::serialize(s);
}
