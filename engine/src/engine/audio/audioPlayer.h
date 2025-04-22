#pragma once

#include "engine/core/Window.h"
#include <string>

namespace Engine {

	class AudioPlayer
	{
	public:
		~AudioPlayer() {}
		
		static Scope<AudioPlayer> Create();

		virtual bool PlaySound(std::string filePath, bool loop) = 0;
		virtual void UpdateAudio() = 0;
	};
}