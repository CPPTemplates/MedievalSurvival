#include "settings.h"
#include "musicManager.h"
#include "musicList.h"
#include "sound/soundHandler2D.h"
std::shared_ptr<audio2d> currentlyPlayingMusic = nullptr;
const audioCollection *currentlyPlayingCollection = nullptr;
void updateMusic(const audioCollection *const &musicIfFinished)
{
	if (!currentlyPlayingMusic || (currentlyPlayingMusic->audioLoaded() && currentlyPlayingMusic->getStatus() == sf::SoundSource::Status::Stopped) || !settings::soundSettings::playMusic)
	{
		stopMusic();
		if (settings::soundSettings::playMusic && musicIfFinished)
		{
			playMusic(musicIfFinished);
		}
	}
}
void stopMusic()
{
	if (currentlyPlayingMusic)
	{
		currentlyPlayingMusic->stop();
		//delete this reference
		currentlyPlayingMusic = nullptr;
	}
}
void replaceMusic(const audioCollection *const &newMusic)
{
	if (newMusic != currentlyPlayingCollection)
	{
		stopMusic();
		playMusic(newMusic);
	}
}
void playMusic(const audioCollection *const &newMusic)
{
	currentlyPlayingCollection = newMusic;
	currentlyPlayingMusic = newMusic->playRandomAudioToClient();
}