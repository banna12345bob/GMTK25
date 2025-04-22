#include "SDL_AudioPlayer.h"

namespace Engine {

	SDL_AudioPlayer::SDL_AudioPlayer()
	{
		if (!SDL_Init(SDL_INIT_AUDIO)) {
			EG_CORE_FATAL("SDL could not initialize audio! SDL_Error: {0}", SDL_GetError());
			EG_CORE_ASSERT(false, "SDL ERROR");
		}

		m_deviceId = SDL_OpenAudioDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, NULL);
		if (m_deviceId == 0) {
			EG_CORE_FATAL("Couldn't open audio device! SDL_Error: {0}", SDL_GetError());
			EG_CORE_ASSERT(false, "SDL ERROR");
		}

		SDL_GetAudioDeviceFormat(m_deviceId, &m_deviceSpec, NULL);
	}

	void SDL_AudioPlayer::UpdateAudio()
	{
		for (auto it = m_sounds.begin(); it != m_sounds.end(); )
		{
			if (SDL_GetAudioStreamQueued(it->stream) < it->bufferSize) {
				uint32_t bytesToPut = std::min(it->bufferSize, it->dataLen - it->currentOffset);

				// Apply volume control
				std::vector<uint8_t> output(bytesToPut);
				SDL_MixAudio(output.data(), it->data + it->currentOffset, it->format, bytesToPut, it->volume);

				if (!SDL_PutAudioStreamData(it->stream, output.data(), bytesToPut)) {
					EG_CORE_ERROR("Failed to put audio data onto the buffer. Error: {0}", SDL_GetError());
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

	// Volume is a float between 0 and 1.
	bool SDL_AudioPlayer::PlaySound(std::string stringPath, bool loop, float_t volume)
	{
		bool retval = false;
		Sound sound = Sound();
		sound.currentOffset = 0;
		sound.loop = loop;
		sound.volume = std::clamp(volume, 0.0f, 1.0f);

		SDL_AudioSpec srcspec;

		// Load file
		const char* filePath = stringPath.c_str();
		if (!SDL_LoadWAV(filePath, &srcspec, &sound.data, &sound.dataLen)) {
			EG_CORE_WARN("Couldn't load .wav file '{0}'. Error: {1}", filePath, SDL_GetError());
			return false;
		}

		sound.format = srcspec.format;

		// Create and bind audio stream
		sound.stream = SDL_CreateAudioStream(&srcspec, &m_deviceSpec);
		if (!sound.stream) {
			EG_CORE_ERROR("Couldn't create audio stream. Error: {0}", SDL_GetError());
			return false;
		}
		else if (!SDL_BindAudioStream(m_deviceId, sound.stream)) {
			EG_CORE_ERROR("Failed to bind '{0}' to device. Error: {1}", filePath, SDL_GetError());
			return false;
		}

		// Set buffer size in bytes. Will be equal to 1/2 a second.
		uint32_t bufferSizeInSamples = srcspec.freq / 2;
		uint32_t sampleSize = SDL_AUDIO_BYTESIZE(srcspec.format);
		sound.bufferSize = bufferSizeInSamples * sampleSize;

		m_sounds.push_back(sound);

		return true;
	}
}