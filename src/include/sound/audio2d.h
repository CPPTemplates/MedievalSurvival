#pragma once
#include "globalFunctions.h"
#include "math/timemath.h"
#include "math/vector/vectn.h"
#include <SFML/Audio/SoundSource.hpp>
#include "interface/idestructable.h"
//#include "AL/al.h"

struct audio2d
{
	virtual void play() = 0;
	virtual void stop() = 0;

	vec2 pos = vec2();
	vec2 speed = vec2();
	fp volume = 1;
	fp pitch = 1;
	bool isSpatial = true;
	bool shouldLoop = true;
	microseconds startedPlaying = 0; // will be set by the first next update

	audio2d(cvec2& pos, cfp& volume, cfp& pitch, cbool& isSpatial, cbool& shouldLoop) : pos(pos), volume(volume), pitch(pitch), isSpatial(isSpatial), shouldLoop(shouldLoop), startedPlaying(0) {}

	virtual microseconds getDuration() = 0;
	virtual bool audioLoaded() const = 0;
	virtual void loadAudio() = 0;
	virtual void unLoadAudio() = 0;

	virtual void setVolume(cfp& volume) = 0;
	virtual void setPitch(cfp& pitch) = 0;
	virtual void setMinDistance(cfp& minDistance) = 0;
	virtual void setAttenuation(cfp& attenuation) = 0;
	virtual void setPlayingOffset(const microseconds& offset) = 0;
	virtual void setPosition(cvec2& newPosition);
	virtual microseconds getPlayingOffset() = 0;

	virtual sf::SoundSource::Status getStatus() const = 0;
};

template <typename audioType>
struct audio2dt : audio2d, IDestructable
{
	audioType* playingAudio = nullptr;
	virtual bool audioLoaded() const override;
	virtual sf::SoundSource::Status getStatus() const override;
	virtual void unLoadAudio() override;

	virtual void setMinDistance(cfp& minDistance) override;
	virtual void setAttenuation(cfp& attenuation) override;
	virtual void setVolume(cfp& volume) override;
	virtual void setPitch(cfp& pitch) override;
	virtual void setPlayingOffset(const microseconds& offset) override;
	virtual microseconds getPlayingOffset() override;
	virtual void setPosition(cvec2& newPosition) override;
	inline void setVelocity(cvec2& newSpeed) const
	{
		playingAudio->setVelocity({ (float)newSpeed.x, (float)newSpeed.y, 0.f });
	}

	virtual void play() override;
	virtual void stop() override;

	audio2dt(cvec2& pos, cfp& volume, cfp& pitch, cbool& isSpatial, cbool& shouldLoop) : audio2d(pos, volume, pitch, isSpatial, shouldLoop) {}

	inline ~audio2dt() override
	{
		delete playingAudio;
		playingAudio = nullptr;
	}
};
template <typename T>
inline bool audio2dt<T>::audioLoaded() const
{
	return playingAudio;
}

template <typename T>
inline sf::SoundSource::Status audio2dt<T>::getStatus() const
{
	assumeInRelease((bool)playingAudio);
	return playingAudio->getStatus();
}

template <typename T>
inline void audio2dt<T>::unLoadAudio()
{
	if (playingAudio)
	{
		playingAudio->stop();
		delete playingAudio;
		playingAudio = nullptr;
	}
}

template <typename T>
inline void audio2dt<T>::setMinDistance(cfp& minDistance)
{
	playingAudio->setMinDistance((float)minDistance);
}

template <typename T>
inline void audio2dt<T>::setAttenuation(cfp& attenuation)
{
	playingAudio->setAttenuation((float)attenuation);
}

template <typename T>
inline void audio2dt<T>::setVolume(cfp& newVolume)
{
	if (audio2d::volume != newVolume)
	{

		audio2d::volume = newVolume;
		if (playingAudio)
		{
			playingAudio->setVolume((float)newVolume * 100.0f);
		}
	}
}
template <typename T>
inline void audio2dt<T>::setPitch(cfp& newPitch)
{
	audio2d::pitch = newPitch;
	if (playingAudio)
	{
		playingAudio->setPitch((float)newPitch);
	}
}

template <typename T>
inline void audio2dt<T>::setPlayingOffset(const microseconds& offset)
{
	playingAudio->setPlayingOffset(sf::microseconds(offset));
}

template <typename T>
inline microseconds audio2dt<T>::getPlayingOffset()
{
	return playingAudio->getPlayingOffset().asMicroseconds();
}

template <typename T>
inline void audio2dt<T>::setPosition(cvec2& newPosition)
{
	playingAudio->setPosition({ (float)newPosition.x, (float)newPosition.y, 0 });
	audio2d::setPosition(newPosition);
}

template <typename T>
inline void audio2dt<T>::play()
{
	playingAudio->play();
	if (pitch != 1)
	{
		playingAudio->setPitch((float)pitch);
	}
	playingAudio->setVolume((float)volume * 100.0f);
	playingAudio->setLooping(shouldLoop);
	playingAudio->setSpatializationEnabled(isSpatial);
	if (isSpatial)
	{
		playingAudio->setPosition({ (float)pos.x, (float)pos.y, 0 });
		setVelocity(speed);
	}
}

template <typename T>
inline void audio2dt<T>::stop()
{
	playingAudio->stop();
}