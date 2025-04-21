#include "Window.h"

#include "platform/SDL/SDLWindow.h"

namespace Engine {

	std::unique_ptr<Window> Window::Create(WindowProps props)
	{
		return std::make_unique<SDLWindow>(props);
	}
}
