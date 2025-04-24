#pragma once

#include "engine/audio/audioPlayer.h"

#include <SDL3/SDL.h>

#include <string>
#include <vector>

namespace Engine {

	class SDL_AudioPlayer : public AudioPlayer
	{
	public:
		SDL_AudioPlayer();
		virtual bool PlaySound(std::string filePath, bool loop, float_t volume, unsigned int* id) override;
		virtual void UpdateAudio() override;
		virtual void SetLooping(int id, bool value) override;
		virtual void SetVolume(int id, float_t value) override;
		virtual void StopSound(int id) override;

	private:
		SDL_AudioDeviceID m_deviceId;
		SDL_AudioSpec m_deviceSpec;

		struct Sound {
			uint8_t* data;
			uint32_t dataLen;
			uint32_t currentOffset;		// Bytes
			uint32_t bufferSize;		// Bytes
			bool loop;					// Putting this after stream breaks the code, probably alignment issues
			SDL_AudioFormat format;
			float_t volume;
			SDL_AudioStream* stream;
		};

		std::map<int, Sound> m_sounds;
		int m_nextId;
	};
}