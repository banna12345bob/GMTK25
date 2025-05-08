#include "Application.h"

#include <SDL3/SDL.h>

namespace Engine {

	Application::Application(WindowProps props)
	{
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
