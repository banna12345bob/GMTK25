#include "Application.h"

#include <SDL3/SDL.h>

namespace Engine {

	Application::Application(WindowProps props)
	{
		m_Window = Window::Create(props);
	}

	Application::~Application()
	{

	}

	void Application::Run()
	{
		while (running) {
			// Eventually wanna abstract this to its own input handler class that is independent of SDL
			SDL_Event e;

			if (SDL_PollEvent(&e)) {
				switch (e.type) {
				case SDL_EVENT_QUIT:
					this->running = false;
					break;
				}
			}

			this->RunApp();
		}
	}
}
