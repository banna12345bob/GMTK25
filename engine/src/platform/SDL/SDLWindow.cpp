#include "SDLWindow.h"

#include "engine/core/core.h"

namespace Engine {

	SDLWindow::SDLWindow(WindowProps props)
	{
		m_data.title = props.title;
		m_data.width = props.width;
		m_data.height = props.height;
		m_data.fullscreen = props.fullscreen;
		uint32_t WindowFlags = SDL_WINDOW_OPENGL;

		//Initialize SDL
		if (!SDL_Init(SDL_INIT_VIDEO))
		{
			EG_CORE_FATAL("SDL could not initialize! SDL_Error: {0}", SDL_GetError());
			EG_CORE_ASSERT(false, "SDL ERROR");
		}

		if (m_data.fullscreen) {
			int displaycount;
			const SDL_DisplayMode* DM = SDL_GetCurrentDisplayMode(SDL_GetDisplays(&displaycount)[0]);
			m_data.width = DM->w;
			m_data.height = DM->h;
		}

		m_window = SDL_CreateWindow(m_data.title.c_str(), m_data.width, m_data.height, WindowFlags);

		if (m_window == NULL)
		{
			EG_CORE_FATAL("Window could not be created! SDL_Error: {0}", SDL_GetError());
			EG_CORE_ASSERT(false, "SDL ERROR");
		}

		if (m_data.fullscreen)
			SDL_SetWindowFullscreen(m_window, SDL_WINDOW_FULLSCREEN);
	}

}
