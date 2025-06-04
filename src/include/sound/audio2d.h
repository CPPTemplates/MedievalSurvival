#pragma once
#include "globalFunctions.h"
#include "math/timemath.h"
#include "math/vector/vectn.h"
#include <SFML/Audio/SoundSource.hpp>
#include "interface/idestructable.h"
#include "AudioType.h"
#include "AudioSource.h"
#include <filesystem/sfmlInputStream.h>
#include <filesystem/fileio.h>
//#include "AL/al.h"

struct audio2d
{
	std::shared_ptr<AudioSource> source;
	sf::SoundSource* playingAudio = nullptr;
	sfmlInputStream* stream = nullptr;
	void play();
	void stop();

	vec2 pos = vec2();
	vec2 speed = vec2();
	fp volume = 1;
	//*100 to go to sfml volume. don't change this unless you know what you're doing!
	fp volumeMultiplier = 100;
	fp pitch = 1;
	fp minDistance = 1;
	fp attenuation = 1;
	bool isSpatial = true;
	bool shouldLoop = true;
	microseconds startedPlaying = 0; // will be set by the first next update
	microseconds duration = 0; //will be set when the audio is loaded

	audio2d(const std::shared_ptr<AudioSource>& source, cvec2& pos = vec2(), cfp& volume = 1, cfp& pitch = 1, cbool& isSpatial = true, cbool& shouldLoop = false) : source(source), pos(pos), volume(volume), pitch(pitch), isSpatial(isSpatial), shouldLoop(shouldLoop), startedPlaying(0) {}

	microseconds getDuration();
	bool audioLoaded() const;
	void loadAudio();
	void unLoadAudio();

	void setVolume(cfp& volume);
	void setVolumeMultiplier(cfp& mult);
	void setPitch(cfp& pitch);
	void setMinDistance(cfp& minDistance);
	void setAttenuation(cfp& attenuation);
	void setPlayingOffset(const microseconds& offset);
	void setPosition(cvec2& newPosition);
	microseconds getPlayingOffset();
	void updateVolume() const;


	template<typename lambdaType>
	inline decltype(auto) applyToOriginal(lambdaType lambda) {
		return source->type == AudioType::sound ?
			lambda((sf::Sound*)playingAudio) :
			lambda((sf::Music*)playingAudio);
	}
	~audio2d();
	void setVelocity(cvec2& newSpeed) const;

	sf::SoundSource::Status getStatus() const;
};


