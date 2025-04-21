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
		virtual bool AddSound(std::string filePath) override;
		virtual void UpdateSounds() override;

	private:
		SDL_AudioDeviceID m_deviceId;
		SDL_AudioSpec m_deviceSpec;

		struct Sound {
			uint8_t* data;
			uint32_t dataLen;
			SDL_AudioStream* stream;
		};

		std::vector<Sound> m_sounds;
	};
}