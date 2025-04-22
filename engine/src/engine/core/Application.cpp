#include "Application.h"

#include <SDL3/SDL.h>

namespace Engine {

	Application::Application(WindowProps props)
	{
		m_Window = Window::Create(props);
		m_EventHandler = EventHandler::Create(&m_Window);
		m_AudioPlayer = AudioPlayer::Create();

		m_AudioPlayer->PlaySound("assets/audio/music/music1_short.wav", false, 0.4f);
	}

	Application::~Application()
	{

	}

	void Application::Run()
	{
		while (m_Window->GetRunning()) {
			m_EventHandler->HandleEvents();
			m_AudioPlayer->UpdateAudio();

			this->UpdateApp();
		}
	}
}
