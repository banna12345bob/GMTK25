#include "eventCallbackManager.h"

#include "engine/core/Log.h"

namespace Engine {

	eventCallbackManager::eventCallbackManager()
	{
		m_KeyboardCallbacks = new std::vector<void (*)(void*)>();
	}

	void eventCallbackManager::registerKeyboardCallback(void (*callback)(void*))
	{
		m_KeyboardCallbacks->push_back(callback);
	}
}

