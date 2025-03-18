#pragma once
#include "audio2d.h"
#include <SFML/Audio/Music.hpp>
#include "filesystem/filemanager.h"
class sfmlInputStream;
struct music2d : audio2dt<sf::Music>
{
	stdPath path;
	sfmlInputStream *stream = nullptr;

	inline music2d(const stdPath &path, cvec2 &pos, cfp &volume, cfp &pitch, cbool &isSpatial) : audio2dt(pos, volume, pitch, isSpatial), path(path)
	{
	}

	virtual void loadAudio() override;
	virtual microseconds getDuration() override;
	~music2d();
};