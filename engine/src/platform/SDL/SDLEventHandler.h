#pragma once

#include "engine/events/eventHandler.h"

#include <SDL3/SDL.h>

namespace Engine {

	class SDLEventHandler : public EventHandler
	{
	public:
		SDLEventHandler();
		virtual void HandleEvents() override;

	};
}

