#include "SDLEventHandler.h"

namespace Engine {

	SDLEventHandler::SDLEventHandler(std::unique_ptr<Window>* window)
	{
		m_Window = window;
	}

	void SDLEventHandler::HandleEvents()
	{
		SDL_Event e;

		if (SDL_PollEvent(&e)) {
			switch (e.type) {
			case SDL_EVENT_QUIT:
				m_Window->get()->SetRunning(false);
				break;
			case SDL_EVENT_KEY_DOWN:
				EG_CORE_WARN("Impliment keyboard events");
				break;
			}
		}
	}
}