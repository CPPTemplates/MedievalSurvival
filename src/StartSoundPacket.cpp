#include "stdafx.h"
#include "StartSoundPacket.h"
#include "nbt/serializeVector.h"
#include "math/random/random.h"
#include "nbt/serializeOptional.h"

bool StartSoundPacket::serialize(nbtSerializer& s)
{
	serializeNBTValue(s, L"position", position);
	s.serializeMembers(L"key", key);
	s.serializeMembers(L"sound index", soundIndex);
	s.serializeMembers(L"volume", volume);
	s.serializeMembers(L"pitch", pitch);
	s.serializeMembers(L"shouldLoop", shouldLoop);
	return SoundPacket::serialize(s);
}
