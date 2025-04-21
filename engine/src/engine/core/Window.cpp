#include "Window.h"

#include "platform/SDL/SDLWindow.h"

namespace Engine {

	Scope<Window> Window::Create(WindowProps props)
	{
		return CreateScope<SDLWindow>(props);
	}
}
