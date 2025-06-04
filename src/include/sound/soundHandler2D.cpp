#include "soundHandler2D.h"
#include "filesystem/sfmlInputStream.h"
#include "filesystem/fileio.h"
#include <SFML/Audio/Listener.hpp>
#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include "audio2d.h"
#include "../math/mattnxn.h"
#include "math/graphics/texture.h"
#include "math/graphics/graphicsFunctions.h"
#include "math/graphics/brush/brushes/solidColorBrush.h"

constexpr int maxSources = 0x100;
soundHandler2d handler = soundHandler2d();

// https://www.sfml-dev.org/tutorials/1.6/audio-spatialization.php#:~:text=The%20attenuation%20is%20a%20multiplicative,very%20close%20to%20the%20listener.
static constexpr fp getVolumeFactor(cfp& minDistance, cfp& attenuation, cfp& distance)
{
	// paste this in desmos:
	//       \frac{m}{m\ +\ a\left(\max\left(x,m\right)-m\right)}
	// for example: mindistance = 12, attenuation = 1 , distance = 24 -> factor = 0.5
	return minDistance / (minDistance + attenuation * (math::maximum(distance, minDistance) - minDistance));
}

// reverse engineered above function
// f = m / (m + a (x - m))
// f (m + a (x - m)) = m
// mf + af (x - m) = m
// af (x - m) = m - mf
// af = (m - mf) / (x - m)
// a = (m - mf) / f (x - m)
// a = m (1 - f) / f (x - m)
static constexpr fp getAttenuation(cfp& minDistance, cfp& distance, cfp& desiredFactor)
{
	return (minDistance * (1 - desiredFactor)) / (desiredFactor * (distance - minDistance));
}

static constexpr fp getHeadRadius(cfp& headScreenDistance)
{
	return headScreenDistance / 10;
}
// #ifndef ALC_HRTF_SOFT
// #define ALC_HRTF_SOFT 0x1992
// #endif
void soundHandler2d::update()
{
	static bool initialized = false;
	if (!initialized)
	{
		// alDopplerFactor();
		// very slow velocity, to test
		// 1m / s ... ?



		// Check if HRTF is enabled
		// alcGetIntegerv(device, ALC_HRTF_SOFT, 1, &hrtf);
		// if (hrtf)
		//{
		//    printf("HRTF is enabled\n");
		//    auto name = alcGetString(device, ALC_HRTF_STATUS_SOFT);
		//    printf("HRTF status: %s\n", name);
		//}
		// else
		//{
		//    printf("HRTF is not enabled\n");
		//}
		//// alcGetIntegerv(device, ALC_HRTF_SOFT, 1, &hrtf);
		//
		// initialized = true;
	}

	cfp& hearingRange3d = cvec2(hearingRange, earPosition.z).length();
	sf::Listener::setPosition({ (float)earPosition.x, (float)earPosition.y, (float)earPosition.z });
	sf::Listener::setVelocity({ (float)earSpeed.x, (float)earSpeed.y, (float)earSpeed.z });
	sf::Listener::setDirection({ 0.0f, 0.0f, -1.0f }); //-z is at the back of the screen
	sf::Listener::setUpVector({ 0.0f, 1.0f, 0.0f });

	cmicroseconds currentTime = getmicroseconds();
	currentlyPlayIngAudio.update();
	// below the minimal volume, sounds will cut out. cannot be 0 because attenuation would have to be infinite
	cfp& minimalVolume = 0.05f;
	cfp& headSize = getHeadRadius(earPosition.z); // the distance between the head and the screen is normally 3 * the size of the head

	// the place closest to the ear position, but still on the screen
	cfp& closestDistance = earPosition.z;

	// when distance <= minDistance, you will hear the sound at the maximum volume
	// choose closestDistance if you want sources at the closest point on screen to be fully heard
	cfp& minDistance = closestDistance;

	cfp& attenuation = getAttenuation(minDistance, hearingRange3d, minimalVolume);
	cfp& volumeFactorAtMinDistance = getVolumeFactor(minDistance, attenuation, closestDistance);
	cfp& volumeFactorAtMaxDistance = getVolumeFactor(minDistance, attenuation, hearingRange3d);
	cfp& volumeMultiplier = 100.0f;// / volumeFactorAtMinDistance;

	sf::Listener::setGlobalVolume((float)globalVolume * 100.0f);

	for (size_t i = 0; i < currentlyPlayIngAudio.size; i++)
	{
		std::shared_ptr<audio2d> s = currentlyPlayIngAudio[i];

		cfp& distance3D = (vec3(s->pos) - earPosition).length();
		cbool& canHear = (distance3D <= hearingRange3d) || (!s->isSpatial);
		// enable this for debugging purposes
		// cfp &volumeMultiplier = getVolumeFactor(minDistance, attenuation, distance3D);

		if (s->startedPlaying)
		{
			// const microseconds soundDuration = 1000000;
			//  check if stopped
			if (s->audioLoaded())
			{
				if (s->getStatus() == sf::SoundSource::Status::Stopped)
				{
					// remove from list
					playingSoundCount--;
					currentlyPlayIngAudio.erase(i);
					continue;
				}
				else
				{
				}
			}
			else
			{
				const microseconds soundDuration = s->getDuration();
				if (s->startedPlaying + soundDuration < currentTime)
				{
					currentlyPlayIngAudio.erase(i);
					continue;
				}
			}
		}
		else
		{
			s->startedPlaying = currentTime;
		}
		// check position
		if (s->audioLoaded() && (s->getStatus() == sf::SoundSource::Status::Playing))
		{
			if (!canHear)
			{
				playingSoundCount--;
				s->unLoadAudio();
				continue;
			}
		}
		else
		{
			// half the maximum
			if (canHear && (playingSoundCount < maxSources))
			{
				playingSoundCount++;
				if (!s->audioLoaded())
				{
					s->loadAudio();
				}
				s->play();


				//alSourcef(s->getSource(), AL_ROLLOFF_FACTOR, 0.5f);
				if (s->isSpatial)
				{
					s->setMinDistance(minDistance);
					s->setAttenuation(attenuation);
					// Set the Doppler factor (how pronounced the effect is)

					//sf::Sound::setDopplerFactor(1.0f); // Default value

					// Set the reference velocity for Doppler calculations
					//alDopplerVelocity(1.0f); // Typically 1.0, but can be adjusted

					// Set the speed of sound (default is 343.3 m/s in air at 20°C)
					//alSpeedOfSound(343.3f);
				}
				s->setPlayingOffset(currentTime - s->startedPlaying);
			}
			else
			{
			}
		}
		if (canHear)
		{
			//s->setVolume(1.0f);
			if (s->startedPlaying && s->isSpatial && s->audioLoaded())
				s->setVolumeMultiplier(volumeMultiplier);
			//s->playingSound->setVolume(s->volume * volumeMultiplier * 100);
		}
	}
}

soundHandler2d::soundHandler2d()
{
}

void soundHandler2d::playAudio(const std::shared_ptr<audio2d>& audioToPlay)
{
	currentlyPlayIngAudio.push_back(audioToPlay);
}

void soundHandler2d::visualize(const texture& renderTarget)
{
	crectangle2& screenRect = rectangle2(renderTarget.getClientRect().getCenter(), vec2()).expanded((fp)renderTarget.size.maximum() / 4);
	constexpr bool drawSideView = true;
	cfp& currentHearingRange = drawSideView ? cvec2(hearingRange, earPosition.z).length() : hearingRange;
	cmat3x3& worldToScreen = mat3x3::fromRectToRect(
		rectangle2((drawSideView ? vec2(earPosition.x, earPosition.z) : vec2(earPosition)) - currentHearingRange, vec2(currentHearingRange * 2)),
		screenRect);
	cfp& maxBlobRadius = screenRect.w / 0x20;
	if (drawSideView)
	{
		// draw head
		cfp& headRadius = getHeadRadius(earPosition.z);
		crectangle2& headScreenRect = worldToScreen.multRectMatrix(rectangle2(vec2(earPosition.x, earPosition.z), vec2()).expanded(headRadius));
		fillEllipse(renderTarget, headScreenRect, colorMixer(solidColorBrush(color(color::quarterMaxValue, color::maxValue, color::halfMaxValue, color::halfMaxValue)), renderTarget));
		crectangle2& audibleScreenRangeRect = worldToScreen.multRectMatrix(rectangle2(vec2(earPosition.x - hearingRange, 0), vec2(hearingRange * 2, 0)));
		fillRectangle(renderTarget, audibleScreenRangeRect.expanded(1), brushes::white);
	}
	// draw circle with hearing distance
	fillEllipse(renderTarget, screenRect, colorMixer(solidColorBrush(color(color::quarterMaxValue, color::maxValue, color::maxValue, color::maxValue)), renderTarget));
	fp offset = 0;
	for (const auto& audio : currentlyPlayIngAudio)
	{
		color c;
		if (audio->audioLoaded())
		{
			c = colorPalette::blue;
		}
		else
		{
			c = colorPalette::red;
		}
		cfp& blobradius = audio->volume * maxBlobRadius;
		auto solidBrush = solidColorBrush(color(c, color::quarterMaxValue));
		auto mixer = colorMixer(solidBrush, renderTarget);
		if (audio->isSpatial)
		{
			cvec2& screenPos = worldToScreen.multPointMatrix(drawSideView ? vec2(audio->pos.x, 0) : audio->pos);
			fillEllipse(renderTarget, crectangle2(screenPos, vec2()).expanded(blobradius), mixer);
			cfp& miniAudioMultiplier = getVolumeFactor(audio->minDistance, audio->attenuation, (earPosition - vec3(audio->pos.x, audio->pos.y, 0)).length());
			cfp& soundHandlerMultiplier = audio->volume * audio->volumeMultiplier;
			cfp& finalVolume = miniAudioMultiplier * soundHandlerMultiplier * 0.01;
			fillEllipse(renderTarget, crectangle2(screenPos, vec2()).expanded(finalVolume * maxBlobRadius), mixer);
		}
		else
		{
			fillRectangle(renderTarget, crectangle2(vec2(0, offset), vec2(blobradius)), mixer);
			offset += maxBlobRadius + 2;
		}
	}
}

soundHandler2d::~soundHandler2d()
{
}
