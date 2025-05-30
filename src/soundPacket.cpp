#include "soundPacket.h"
#include <string>
#include "nbt/nbtSerializer.h"
#include "nbt/serializeVector.h"
#include "nbt/serializeUUID.h"
#include "StartSoundPacket.h"
#include "UpdateSoundPacket.h"
#include "StopSoundPacket.h"

bool SoundPacket::serialize(nbtSerializer& s)
{
	s.serializeMembers(L"type", type);
	return serializeNBTValue(s, L"id", id);
}

SoundPacket* getPacket(nbtSerializer& s)
{
	SoundPacketType type;
	s.serializeMembers(L"type", type);
	SoundPacket* r;
	uuid id;
	serializeNBTValue(s, L"id", id);
	switch (type)
	{
	case start:
		r = new StartSoundPacket(id);
		break;
	case update:
		r = new UpdateSoundPacket(id);
		break;
	case stop:
		r = new StopSoundPacket(id);
		break;
	default:
		return nullptr;
		break;
	}
	if (r->serialize(s)) {
		return r;
	}
	delete r;
	return nullptr;
}
