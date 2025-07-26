#include "Application.h"

#include "engine/debug/Instrumentor.h"

#include "engine/core/Keycodes.h"
#include "engine/events/Key.h"

namespace Engine {

	static Application* s_Instance = nullptr;

	Application::Application(WindowProps props)
	{
		EG_PROFILE_FUNCTION();
		s_Instance = this;

		m_Window = Window::Create(props);
		m_EventCallbackManager = new eventCallbackManager();
		m_AudioPlayer = AudioPlayer::Create();
		m_ImGuiRenderer = new ImGuiRenderer();

		m_Window->SetVSync(false);

		getImGuiRenderer()->registerImGuiLayer(m_AudioDebuggerLayer);

		m_EventCallbackManager->registerKeyboardCallback(AudioDebuggerKeyboardEventCallback);
	}

	Application::~Application()
	{
		EG_PROFILE_FUNCTION();
	}

	Application* Engine::Application::getApplication()
	{
		return s_Instance;
	}

	void Application::AudioDebuggerKeyboardEventCallback(void* callback) {
		EG_PROFILE_FUNCTION();
		if ((Key::isKeyPressed(EG_SCANCODE_LCTRL) || Key::isKeyPressed(EG_SCANCODE_RCTRL)) && Key::wasKeyPressed(EG_SCANCODE_P))
			Application::getApplication()->m_AudioDebuggerLayer->m_ShowWindow = !Application::getApplication()->m_AudioDebuggerLayer->m_ShowWindow;
	}

	void Application::Run()
	{
		EG_PROFILE_FUNCTION();
		int a, b, deltaTime;
		b = 0;
		while (m_Window->GetRunning()) {
			a = (int)SDL_GetTicks();
			deltaTime = a - b;

			if (deltaTime != 0)
				m_frameRate = 1000 / deltaTime;

			m_Window->Update();

			this->UpdateApp();
			b = a;
		}
	}
}
