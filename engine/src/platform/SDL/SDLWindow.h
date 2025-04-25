#pragma once

#include "engine/core/Window.h"

#include <SDL3/SDL.h>
#include <SDL3/SDL_opengl.h>

namespace Engine {

	class SDLWindow : public Window
	{
	public:
		SDLWindow(WindowProps props);
		virtual ~SDLWindow();

		virtual int GetWidth() override  { return m_data.width; }
		virtual int GetHeight() override { return m_data.height; }

		virtual void SetWidth(int width) override;
		virtual void SetHeight(int height) override;

		virtual void ReloadWindow() override;

	private:
		SDL_Window* m_window;

		struct WindowData {
			std::string title;
			int width, height;
			bool fullscreen;

			const char* pathToIcon;
		};

		WindowData m_data;
	};

}

