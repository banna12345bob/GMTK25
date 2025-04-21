#pragma once

#include "core.h"
//#include <string>
//#include <sstream>
//#include <map>

#include "engine/core/Window.h"
#include "engine/events/eventHandler.h"

namespace Engine {

	class Application
	{
	public:
        Application(WindowProps props);
		virtual ~Application();

		virtual void Run();

		virtual void UpdateApp() = 0;
	private:
		Scope<Window> m_Window;
		Scope<EventHandler> m_EventHandler;
	};

	//	To be defined in client
    Application* CreateApplication();
}
