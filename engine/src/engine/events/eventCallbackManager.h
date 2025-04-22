#pragma once
#include <vector>
#include <SDL3/SDL.h>


namespace Engine {
	class eventCallbackManager
	{
	public:
		eventCallbackManager();
		void registerKeyboardCallback(void (*callback)(SDL_Scancode));

		std::vector<void (*)(SDL_Scancode)>* getKeyboardCallbacks() { return m_KeyboardCallbacks; }
	private:
		std::vector<void (*)(SDL_Scancode)>* m_KeyboardCallbacks;
	};
}

