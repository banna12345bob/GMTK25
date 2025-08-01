#pragma once

#include "core.h"

#include "engine/core/Window.h"
#include "engine/audio/audioPlayer.h"

#include "engine/events/eventCallbackManager.h"

#include "engine/audio/AudioDebugger.h"

#include "engine/ImGui/ImGuiRenderer.h"

#include "Layer.h"
#include "LayerStack.h"

namespace Engine {

	class Application
	{
	public:
        Application(WindowProps props);
		virtual ~Application();

		static Application* getApplication();

		static void AudioDebuggerKeyboardEventCallback(void* callback);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);

		virtual void Run();

		Scope<Window>& getWindow() { return m_Window; }
		Scope<AudioPlayer>& getAudioPlayer() { return m_AudioPlayer; }
		eventCallbackManager* getCallbackManager() { return m_EventCallbackManager; }
		ImGuiRenderer* getImGuiRenderer() { return m_ImGuiRenderer; }

		int m_frameRate = 0;
	private:
		Scope<Window> m_Window;
		Scope<AudioPlayer> m_AudioPlayer;
		eventCallbackManager* m_EventCallbackManager;
		ImGuiRenderer* m_ImGuiRenderer;

		AudioDebugger* m_AudioDebuggerLayer = new AudioDebugger();

		LayerStack m_layerStack;
	};

	//	To be defined in client
    Application* CreateApplication();
}
