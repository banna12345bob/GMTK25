#pragma once
#include <vector>
#include <SDL3/SDL.h>

namespace Engine {
	class eventCallbackManager
	{
	public:
		eventCallbackManager();
		void registerKeyboardCallback(void (*callback)(void*));

		std::vector<void (*)(void*)>* getKeyboardCallbacks() { return m_KeyboardCallbacks; }
	private:
		std::vector<void (*)(void*)>* m_KeyboardCallbacks;
	};
}

