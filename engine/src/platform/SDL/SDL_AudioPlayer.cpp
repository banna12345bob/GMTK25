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
		for (auto it = m_sounds.begin(); it != m_sounds.end(); )
		{
			if (SDL_GetAudioStreamQueued(it->stream) < it->bufferSize) {
				uint32_t bytesToPut = std::min(it->bufferSize, it->dataLen - it->currentOffset);

				if (!SDL_PutAudioStreamData(it->stream, it->data + it->currentOffset, bytesToPut)) {
					SDL_Log("Failed to put audio data onto the buffer: %s", SDL_GetError());
					m_sounds.erase(it);
					it--;
					continue;
				}

				it->currentOffset += bytesToPut;

				if (it->currentOffset >= it->dataLen) {
					if (it->loop) {
						it->currentOffset = 0;
					}
					else {
						it = m_sounds.erase(it);
						continue;
					}
				}
			}

			it++;
		}
	}

	bool SDL_AudioPlayer::PlaySound(std::string stringPath, bool loop)
	{
		bool retval = false;
		Sound sound = Sound();
		sound.currentOffset = 0;
		sound.loop = loop;

		SDL_AudioSpec srcspec;

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
			return false;
		}
		else if (!SDL_BindAudioStream(m_deviceId, sound.stream)) {
			SDL_Log("Failed to bind '%s' to device: %s", filePath, SDL_GetError());
			return false;
		}

		// Set buffer size in bytes. Will be equal to 1/2 a second.
		uint32_t bufferSizeInSamples = srcspec.freq / 2;
		uint32_t sampleSize = SDL_AUDIO_BYTESIZE(srcspec.format);
		sound.bufferSize = bufferSizeInSamples * sampleSize;

		m_sounds.push_back(sound);

		SDL_free(fullPath);
		return true;
	}
}