#include "SDLEventHandler.h"

#include "engine/events/Key.h"

#include <backends/imgui_impl_sdl3.h>

namespace Engine {

	SDLEventHandler::SDLEventHandler(Scope<Window>* window, eventCallbackManager* eventCallbackManager)
	{
		m_Window = window;
		m_EventCallbackManager = eventCallbackManager;

	}

	void SDLEventHandler::HandleEvents()
	{
		SDL_Event e;
	
		if (SDL_PollEvent(&e)) {
			ImGui_ImplSDL3_ProcessEvent(&e);
			switch (e.type) {
			case SDL_EVENT_QUIT:
				m_Window->get()->SetRunning(false);
				break;
			case SDL_EVENT_WINDOW_RESIZED:
				// TODO: Change OpenGL viewport size
				m_Window->get()->ReloadWindow();
				EG_CORE_INFO("W: {0}, H: {1}", m_Window->get()->GetWidth(), m_Window->get()->GetHeight());
				break;
			case SDL_EVENT_WINDOW_MINIMIZED:
				// TODO: Add a config option that caps the FPS if window minimized
				EG_CORE_WARN("implement Window Minimized event");
				break;
			case SDL_EVENT_WINDOW_RESTORED:
				EG_CORE_WARN("implement Window Restored events");
				break;
			case SDL_EVENT_KEY_DOWN:
				Key::setKeyPressed(e.key.scancode, true);
				if (m_EventCallbackManager->getKeyboardCallbacks()->size() == 0) {
					EG_CORE_WARN("No keyboard callbacks registered");
					break;
				}
				for (int i = 0; i < m_EventCallbackManager->getKeyboardCallbacks()->size(); i++)
				{
					m_EventCallbackManager->getKeyboardCallbacks()->at(i)(nullptr);
				}
				break;
			case SDL_EVENT_KEY_UP:
				Key::setKeyPressed(e.key.scancode, false);
				break;
			}
		}
	}
}