#include "Application.h"

#include <SDL3/SDL.h>

namespace Engine {

	Application::Application()
	{
		m_Window = Window::Create();
	}

	Application::~Application()
	{

	}

	void Application::Run()
	{
		while (running) {
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
