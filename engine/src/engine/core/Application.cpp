#include "Application.h"

#include <SDL3/SDL.h>
#include <imgui.h>

#include "engine/debug/Instrumentor.h"

namespace Engine {

	Application::Application(WindowProps props)
	{
		EG_PROFILE_FUNCTION();
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows
		ImGui::StyleColorsDark();

		m_Window = Window::Create(props);
		m_EventCallbackManager = new eventCallbackManager();
		m_EventHandler = EventHandler::Create(&m_Window, m_EventCallbackManager);
		m_AudioPlayer = AudioPlayer::Create();
		m_GraphicsAPI = GraphicsAPI::Create(&m_Window);

		m_showFPS = false; // Don't show FPS by default

		m_GraphicsAPI->SetVSync(true);
	}

	Application::~Application()
	{
		EG_PROFILE_FUNCTION();
	}

	void Application::Run()
	{
		EG_PROFILE_FUNCTION();
		int a, b, deltaTime;
		b = 0;
		while (m_Window->GetRunning()) {
			a = SDL_GetTicks();
			deltaTime = a - b;

			if (m_showFPS) {
				EG_INFO("fps: {0}", 1000 / (deltaTime + 1));
			}

			m_EventHandler->HandleEvents();
			m_GraphicsAPI->Render();

			this->UpdateApp();
			b = a;
		}
	}
}
