#pragma once
#include <memory>
#include <string>

#include "engine/core/Window.h"

namespace Engine {

	class EventHandler
	{
	public:
		virtual ~EventHandler() {}

		virtual void HandleEvents() = 0;

		static std::unique_ptr<EventHandler> Create(std::unique_ptr<Window>* window);
	};
}

