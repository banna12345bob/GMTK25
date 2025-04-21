#include "SDLEventHandler.h"

namespace Engine {

	SDLEventHandler::SDLEventHandler(Scope<Window>* window)
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
			case SDL_EVENT_WINDOW_RESIZED:
				// TODO: Change OpenGL viewport size
				m_Window->get()->ReloadWindow();
				EG_INFO("W: {0}, H: {1}", m_Window->get()->GetWidth(), m_Window->get()->GetHeight());
				break;
			case SDL_EVENT_WINDOW_MINIMIZED:
				// TODO: Add a config option that caps the FPS if window minimized
				EG_CORE_WARN("implement Window Minimized event");
				break;
			case SDL_EVENT_WINDOW_RESTORED:
				EG_CORE_WARN("implement Window Restored events");
				break;
			case SDL_EVENT_KEY_DOWN:
				EG_CORE_WARN("Properly implement keyboard events");
				switch (e.key.scancode)
				{
					// Just a little test of resizing the window using keybinds. Will remove once I actually implement keyboard events
				case SDL_SCANCODE_PAGEUP:
					m_Window->get()->SetWidth(m_Window->get()->GetWidth() + 100);
					break;
				case SDL_SCANCODE_PAGEDOWN:
					m_Window->get()->SetWidth(m_Window->get()->GetWidth() - 100);
					break;
				}
				break;
			}
		}
	}
}