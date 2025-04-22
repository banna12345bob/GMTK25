#include "eventHandler.h"

#include "platform/SDL/SDLEventHandler.h"

namespace Engine {

	Scope<EventHandler> EventHandler::Create(Scope<Window>* window, eventCallbackManager* eventCallbackManager)
	{
		return CreateScope<SDLEventHandler>(window, eventCallbackManager);
	}

}
