#pragma once
#include <string>

#include "engine/core/core.h"

namespace Engine {

	struct WindowProps
	{
		std::string title;
		unsigned int width;
		unsigned int height;
		const char* pathToIcon;
		bool fullscreen;

		//Default constructor
		WindowProps(const std::string& title,
			unsigned int width,
			unsigned int height,
			const char* pathToIcon = "",
			bool fullscreen = false)
			: title(title), width(width), height(height), pathToIcon(pathToIcon), fullscreen(fullscreen)
		{
		}
	};

	class Window
	{
	public:
		virtual ~Window() {}

		virtual int GetWidth() = 0;
		virtual int GetHeight() = 0;

		virtual bool GetRunning() { return m_Running; }
		virtual void SetRunning(bool running) { m_Running = running; }

		static Scope<Window> Create(WindowProps props);
	private:
		bool m_Running = true;
	};
}

