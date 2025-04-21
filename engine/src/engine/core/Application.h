#pragma once

#include "core.h"
//#include <string>
//#include <sstream>
//#include <map>

#include "engine/core/Window.h"

namespace Engine {

	class Application
	{
	public:
        Application();
		virtual ~Application();

		virtual void Run();

		virtual void RunApp() = 0;

		bool running = true;

	private:
		std::unique_ptr<Window> m_Window;
	};

	//	To be defined in client
    Application* CreateApplication();
}
