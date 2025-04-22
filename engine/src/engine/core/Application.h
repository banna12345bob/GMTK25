#pragma once

#include "core.h"
//#include <string>
//#include <sstream>
//#include <map>

#include "engine/core/Window.h"
#include "engine/events/eventHandler.h"
#include "engine/audio/audioPlayer.h"

#include "engine/events/eventCallbackManager.h"

namespace Engine {

	class Application
	{
	public:
        Application(WindowProps props);
		virtual ~Application();

		virtual void Run();

		virtual void UpdateApp() = 0;

		Scope<Window> m_Window;
		Scope<EventHandler> m_EventHandler;
		Scope<AudioPlayer> m_AudioPlayer;
		eventCallbackManager* m_EventCallbackManager;
	};

	//	To be defined in client
    Application* CreateApplication();
}
