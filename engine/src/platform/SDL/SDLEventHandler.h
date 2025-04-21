#pragma once

#include "engine/events/eventHandler.h"

#include <SDL3/SDL.h>

namespace Engine {

	class SDLEventHandler : public EventHandler
	{
	public:
		SDLEventHandler(Scope<Window>* window);
		virtual void HandleEvents() override;

	private:
		std::unique_ptr<Window>* m_Window;
	};
}

