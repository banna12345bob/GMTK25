#pragma once

#include "engine/core/Window.h"
#include <SDL3/SDL_video.h>

namespace Engine {
	class SpriteRenderer
	{
	public:
		static Scope<SpriteRenderer> Create();

		virtual void Render() {}
	};
}