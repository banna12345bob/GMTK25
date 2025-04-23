#pragma once

#include "engine/core/Window.h"
#include <string>

namespace Engine {

	class AudioPlayer
	{
	public:
		~AudioPlayer() {}
		
		static Scope<AudioPlayer> Create();

		/**
		@param stringPath must include full path, including the file extension.
		@param volume must be a float between 0 and 1.
		@param id a pointer that will be set with the ID of the sound if successfully created. Can pass in NULL if not needed.
		*/
		virtual bool PlaySound(std::string filePath, bool loop, float_t volume, uint32_t* id) = 0;
		virtual void UpdateAudio() = 0;
		virtual void SetLooping(uint32_t id, bool value) = 0;
		virtual void SetVolume(uint32_t id, float_t value) = 0;
		virtual void StopSound(uint32_t id) = 0;
	};
}