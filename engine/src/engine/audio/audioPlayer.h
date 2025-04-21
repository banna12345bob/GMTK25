#pragma once

#include "engine/core/Window.h"
#include <string>

namespace Engine {

	class AudioPlayer
	{
	public:
		~AudioPlayer() {}
		
		static Scope<AudioPlayer> Create();

		virtual bool AddSound(std::string filePath) = 0;
		virtual void UpdateSounds() = 0;
	};
}