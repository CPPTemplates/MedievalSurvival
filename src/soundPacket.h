#pragma once
#include "nbt/nbtSerializer.h"
#include "math/uuid.h"
enum SoundPacketType {
	start,
	update,
	stop,
	changeVolume
};
struct SoundPacket {
	uuid id;
	SoundPacketType type;
	SoundPacket(const uuid& id, const SoundPacketType& type) :
		id(id), type(type) {}
	virtual bool serialize(nbtSerializer& s);
};

SoundPacket* getPacket(nbtSerializer& s);