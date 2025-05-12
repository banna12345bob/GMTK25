#include "Application.h"

#include <SDL3/SDL.h>
#include <imgui.h>

namespace Engine {

	Application::Application(WindowProps props)
	{
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
	}

	Application::~Application()
	{

	}

	void Application::Run()
	{
		while (m_Window->GetRunning()) {
			m_EventHandler->HandleEvents();
			m_AudioPlayer->UpdateAudio();
			m_GraphicsAPI->Render();

			this->UpdateApp();
		}
	}
}
