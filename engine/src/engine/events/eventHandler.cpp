#include "eventHandler.h"

#include "platform/SDL/SDLEventHandler.h"

namespace Engine {

	Scope<EventHandler> EventHandler::Create()
	{
		return CreateScope<SDLEventHandler>();
	}

}
