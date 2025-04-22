#pragma once

#include "engine/events/eventHandler.h"

#include <SDL3/SDL.h>

namespace Engine {

	class SDLEventHandler : public EventHandler
	{
	public:
		SDLEventHandler(Scope<Window>* window, eventCallbackManager* eventCallbackManager);
		virtual void HandleEvents() override;

	private:
		Scope<Window>* m_Window;
		eventCallbackManager* m_EventCallbackManager;
	};
}

