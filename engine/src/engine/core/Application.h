#pragma once

#include "core.h"
//#include <string>
//#include <sstream>
//#include <map>

#include "engine/core/Window.h"
#include "engine/events/eventHandler.h"
#include "engine/audio/audioPlayer.h"
#include "engine/graphics/GraphicsAPI.h"

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
		Scope<GraphicsAPI> m_GraphicsAPI;
		eventCallbackManager* m_EventCallbackManager;

		bool m_showFPS;
	};

	//	To be defined in client
    Application* CreateApplication();
}
