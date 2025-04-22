#include "SDL_AudioPlayer.h"

namespace Engine {

	SDL_AudioPlayer::SDL_AudioPlayer()
	{
		SDL_Init(SDL_INIT_AUDIO);

		m_deviceId = SDL_OpenAudioDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, NULL);
		if (m_deviceId == 0) {
			SDL_Log("Couldn't open audio device: %s", SDL_GetError());
		}

		SDL_GetAudioDeviceFormat(m_deviceId, &m_deviceSpec, NULL);
	}

	void SDL_AudioPlayer::UpdateAudio()
	{
		for (int i = 0; i < SDL_arraysize(m_sounds); i++)
		{
			// FIXME: Whole file is being put onto the buffer, probably not great for music files
			if (SDL_GetAudioStreamQueued(m_sounds[i].stream) < m_sounds[i].dataLen) {
				SDL_PutAudioStreamData(m_sounds[i].stream, m_sounds[i].data, m_sounds[i].dataLen);
			}
		}
	}

	bool SDL_AudioPlayer::PlaySound(std::string stringPath)
	{
		bool retval = false;
		Sound sound = Sound();

		SDL_AudioSpec srcspec = { SDL_AUDIO_S16, 2, 44100 };

		// Load file
		const char* filePath = stringPath.c_str();
		char* fullPath;
		SDL_asprintf(&fullPath, "%s%s.wav", "assets/audio/", filePath);
		if (!SDL_LoadWAV(fullPath, &srcspec, &sound.data, &sound.dataLen)) {
			SDL_Log("Couldn't load .wav file: %s", SDL_GetError());
			return false;
		}

		// Create and bind audio stream
		sound.stream = SDL_CreateAudioStream(&srcspec, &m_deviceSpec);
		if (!sound.stream) {
			SDL_Log("Couldn't create audio stream: %s", SDL_GetError());
		}
		else if (!SDL_BindAudioStream(m_deviceId, sound.stream)) {
			SDL_Log("Failed to bind '%s' to device: %s", filePath, SDL_GetError());
		}
		else {
			//SDL_Log("Audio stream load: SUCCESS!!");
			m_sounds.push_back(sound);
			retval = true;
		}

		SDL_free(fullPath);
		return retval;
	}
}