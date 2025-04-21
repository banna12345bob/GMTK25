#include "Application.h"

#include <SDL3/SDL.h>

namespace Engine {

	Application::Application(WindowProps props)
	{
		m_Window = Window::Create(props);
		m_EventHandler = EventHandler::Create(&m_Window);
	}

	Application::~Application()
	{

	}

	void Application::Run()
	{
		while (m_Window->GetRunning()) {
			// Eventually wanna abstract this to its own input handler class that is independent of SDL
			m_EventHandler->HandleEvents();

			this->RunApp();
		}
	}
}
