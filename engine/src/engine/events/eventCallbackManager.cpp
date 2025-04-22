#include "eventCallbackManager.h"

#include "engine/core/Log.h"

namespace Engine {

	eventCallbackManager::eventCallbackManager()
	{
		m_KeyboardCallbacks = new std::vector<void (*)(SDL_Scancode)>();
	}

	void eventCallbackManager::registerKeyboardCallback(void (*callback)(SDL_Scancode))
	{
		m_KeyboardCallbacks->push_back(callback);
	}
}

