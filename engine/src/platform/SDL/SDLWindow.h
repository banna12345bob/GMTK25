#pragma once

#include "engine/core/Window.h"

#include <SDL3/SDL.h>

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

		virtual void CreateGLContext() override;
		virtual void GL_SwapWindow() override;

	private:
		SDL_Window* m_window;

		WindowProps m_data;
	};

}

