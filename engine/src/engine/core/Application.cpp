#include "Application.h"

#include <imgui.h>

#include "engine/debug/Instrumentor.h"

#include "engine/core/Keycodes.h"
#include "engine/events/Key.h"

namespace Engine {

	static Application* s_Instance = nullptr;

	Application::Application(WindowProps props)
	{
		EG_PROFILE_FUNCTION();
		s_Instance = this;

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking		
		// Just be mindful that for some reason external ImGUI viewports are laggy when VSync is enabled
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows
		ImGui::StyleColorsDark();

		m_Window = Window::Create(props);
		m_EventCallbackManager = new eventCallbackManager();
		m_EventHandler = EventHandler::Create();
		m_AudioPlayer = AudioPlayer::Create();
		m_GraphicsAPI = GraphicsAPI::Create(&m_Window);

		m_GraphicsAPI->SetVSync(false);

		m_Window->registerImGuiLayer(m_AudioDebuggerLayer);

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
			a = SDL_GetTicks();
			deltaTime = a - b;

			if (deltaTime != 0)
				m_frameRate = 1000 / deltaTime;

			m_EventHandler->HandleEvents();
			m_GraphicsAPI->Render();

			this->UpdateApp();
			b = a;
		}
	}
}
