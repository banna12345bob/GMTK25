#pragma once

#include "core.h"

#include "engine/core/Window.h"
#include "engine/events/eventHandler.h"
#include "engine/audio/audioPlayer.h"
#include "engine/graphics/GraphicsAPI.h"

#include "engine/events/eventCallbackManager.h"

#include "engine/audio/AudioDebugger.h"

namespace Engine {

	class Application
	{
	public:
        Application(WindowProps props);
		virtual ~Application();

		static Application* getApplication();

		static void AudioDebuggerKeyboardEventCallback(void* callback);

		virtual void Run();

		virtual void UpdateApp() = 0;

		Scope<Window> m_Window;
		Scope<EventHandler> m_EventHandler;
		Scope<AudioPlayer> m_AudioPlayer;
		Scope<GraphicsAPI> m_GraphicsAPI;
		eventCallbackManager* m_EventCallbackManager;

		int m_frameRate = 0;
	private:
		AudioDebugger* m_AudioDebuggerLayer = new AudioDebugger();
	};

	//	To be defined in client
    Application* CreateApplication();
}
