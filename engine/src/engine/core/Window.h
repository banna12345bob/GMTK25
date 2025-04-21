#pragma once
#include <memory>
#include <string>

namespace Engine {

	struct WindowProps
	{
		std::string title;
		unsigned int width;
		unsigned int height;
		bool fullscreen;

		//Default constructor
		WindowProps(const std::string& title,
			unsigned int width,
			unsigned int height,
			bool fullscreen = false)
			: title(title), width(width), height(height), fullscreen(fullscreen)
		{
		}
	};

	class Window
	{
	public:
		virtual ~Window() {}

		virtual int GetWidth() = 0;
		virtual int GetHeight() = 0;

		static std::unique_ptr<Window> Create(WindowProps props);
	};
}

