#pragma once

#include "engine/core/Window.h"

#include <SDL3/SDL.h>
#include <SDL3/SDL_opengl.h>

namespace Engine {

	class SDLWindow : public Window
	{
	public:
		SDLWindow();
		int GetWidth() override  { return m_data.width; }
		int GetHeight() override { return m_data.height; }

	private:
		SDL_Window* m_window;

		struct WindowData {
			std::string title;
			int width, height;
			bool fullscreen;
		};

		WindowData m_data;
	};

}

