#pragma once
#include <string>

#include "engine/core/Window.h"

#include "engine/events/eventCallbackManager.h"

namespace Engine {

	class EventHandler
	{
	public:
		virtual ~EventHandler() {}

		virtual void HandleEvents() = 0;

		static Scope<EventHandler> Create();
	};
}

