#include "SDLEventHandler.h"

#include "engine/events/Key.h"

#include <backends/imgui_impl_sdl3.h>

#include "engine/debug/Instrumentor.h"

#include "engine/core/Application.h"

namespace Engine {

	SDLEventHandler::SDLEventHandler()
	{
		EG_PROFILE_FUNCTION();

	}

	void SDLEventHandler::HandleEvents()
	{
		EG_PROFILE_FUNCTION();
		SDL_Event e;
	
		if (SDL_PollEvent(&e)) {
			ImGui_ImplSDL3_ProcessEvent(&e);
			switch (e.type) {
			case SDL_EVENT_QUIT:
				Application::getApplication()->getWindow()->SetRunning(false);
				break;
			case SDL_EVENT_WINDOW_RESIZED:
				// TODO: Change OpenGL viewport size
				Application::getApplication()->getWindow()->ReloadWindow();
				EG_CORE_INFO("W: {0}, H: {1}", Application::getApplication()->getWindow()->GetWidth(), Application::getApplication()->getWindow()->GetHeight());
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
				if (Application::getApplication()->getCallbackManager()->getKeyboardCallbacks()->size() == 0) {
					EG_CORE_WARN("No keyboard callbacks registered");
					break;
				}
				for (int i = 0; i < Application::getApplication()->getCallbackManager()->getKeyboardCallbacks()->size(); i++)
				{
					Application::getApplication()->getCallbackManager()->getKeyboardCallbacks()->at(i)(nullptr);
				}
				break;
			case SDL_EVENT_KEY_UP:
				Key::setKeyPressed(e.key.scancode, false);
				break;
			}
		}
	}
}