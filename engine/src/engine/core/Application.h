#pragma once

#include "core.h"

#include "engine/core/Window.h"
#include "engine/audio/audioPlayer.h"

#include "engine/events/eventCallbackManager.h"

#include "engine/audio/AudioDebugger.h"

#include "engine/ImGui/ImGuiRenderer.h"

namespace Engine {

	class Application
	{
	public:
        Application(WindowProps props);
		virtual ~Application();

		static Application* getApplication();

		static void AudioDebuggerKeyboardEventCallback(void* callback);

		virtual void Run();

		Scope<Window>& getWindow() { return m_Window; }
		Scope<AudioPlayer>& getAudioPlayer() { return m_AudioPlayer; }
		eventCallbackManager* getCallbackManager() { return m_EventCallbackManager; }
		ImGuiRenderer* getImGuiRenderer() { return m_ImGuiRenderer; }

		virtual void UpdateApp() = 0;

		int m_frameRate = 0;
	private:
		Scope<Window> m_Window;
		Scope<AudioPlayer> m_AudioPlayer;
		eventCallbackManager* m_EventCallbackManager;
		ImGuiRenderer* m_ImGuiRenderer;

		AudioDebugger* m_AudioDebuggerLayer = new AudioDebugger();
	};

	//	To be defined in client
    Application* CreateApplication();
}
