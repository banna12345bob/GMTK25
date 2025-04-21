#include "eventHandler.h"

#include "platform/SDL/SDLEventHandler.h"

namespace Engine {

	std::unique_ptr<EventHandler> EventHandler::Create(std::unique_ptr<Window>* window)
	{
		return std::make_unique<SDLEventHandler>(window);
	}

}
