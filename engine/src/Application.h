#pragma once

#include "core.h"
#include <string>
#include <sstream>
#include <map>

namespace Engine {

	class Application
	{
	public:
        Application();
		virtual ~Application();

		virtual void Run() = 0;

		bool running = true;
	};

	//	To be defined in client
    Application* CreateApplication();
}
