#include "soundPacket.h"
#pragma once
struct StopSoundPacket : SoundPacket {
	StopSoundPacket(const uuid& id) : SoundPacket(id, SoundPacketType::stop) {}
};