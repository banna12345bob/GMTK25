#include "eventHandler.h"

#include "platform/SDL/SDLEventHandler.h"

namespace Engine {

	Scope<EventHandler> EventHandler::Create(Scope<Window>* window)
	{
		return CreateScope<SDLEventHandler>(window);
	}

}
