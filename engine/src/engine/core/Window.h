#pragma once
#include <memory>
#include <string>

namespace Engine {

	class Window
	{
	public:
		virtual ~Window() {}

		virtual int GetWidth() = 0;
		virtual int GetHeight() = 0;

		static std::unique_ptr<Window> Create();
	};
}

