#include "Window.h"

#include "platform/SDL/SDLWindow.h"

namespace Engine {

	std::unique_ptr<Window> Window::Create()
	{
		return std::make_unique<SDLWindow>();
		//return std::unique_ptr<Window>();
	}
}
