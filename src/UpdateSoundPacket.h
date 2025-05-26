#include "soundPacket.h"
#pragma once
struct UpdateSoundPacket : SoundPacket {
	std::optional<vec2> newVelocity;
	std::optional<vec2> newPosition;
	std::optional<fp> newVolume = std::nullopt;
	std::optional<fp> newPitch = std::nullopt;
	UpdateSoundPacket(const uuid& id, cvec2& newVelocity = vec2(), cvec2& newPosition = vec2()): newVelocity(newVelocity), newPosition(newPosition), SoundPacket(id, SoundPacketType::update){}
	virtual bool serialize(nbtSerializer& s) override;
};