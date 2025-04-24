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

		m_nextId = 0;
	}

	/**
	@param stringPath must include full path, including the file extension.
	@param loop bool if sound should loop or not
	@param volume must be a float between 0 and 1.
	@param id a pointer that will be set with the ID of the sound if successfully created. (optional)
	*/
	bool SDL_AudioPlayer::PlaySound(std::string stringPath, bool loop, float_t volume, unsigned int* id)
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

		if (id != NULL) {
			*id = m_nextId;
		}

		m_sounds.emplace(m_nextId++, sound);

		return true;
	}

	void SDL_AudioPlayer::UpdateAudio()
	{
		for (auto it = m_sounds.begin(); it != m_sounds.end(); )
		{
			Sound* current = &it->second;

			if (SDL_GetAudioStreamQueued(current->stream) < current->bufferSize)
			{
				uint32_t bytesToPut = std::min(current->bufferSize, current->dataLen - current->currentOffset);

				// Apply volume control
				std::vector<uint8_t> output(bytesToPut);
				SDL_MixAudio(output.data(), current->data + current->currentOffset, current->format, bytesToPut, current->volume);

				if (!SDL_PutAudioStreamData(current->stream, output.data(), bytesToPut)) {
					EG_CORE_ERROR("Failed to put audio data onto the buffer. Error: {0}", SDL_GetError());
					it = m_sounds.erase(it);
					continue;
				}

				current->currentOffset += bytesToPut;

				if (current->currentOffset >= current->dataLen) {
					if (current->loop) {
						current->currentOffset = 0;
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

	void SDL_AudioPlayer::SetLooping(int id, bool value)
	{
		if (m_sounds.count(id)) {
			m_sounds[id].loop = value;
		}
	}

	void SDL_AudioPlayer::SetVolume(int id, float_t value)
	{
		if (m_sounds.count(id)) {
			m_sounds[id].volume = std::clamp(value, 0.0f, 1.0f);
		}
	}

	void SDL_AudioPlayer::StopSound(int id)
	{
		m_sounds.erase(id);
	}
}