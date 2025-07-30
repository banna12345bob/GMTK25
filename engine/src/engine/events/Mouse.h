#pragma once

#include <unordered_map>
#include <SDL3/SDL_mouse.h>

#include <engine/utils/Vector2D.h>

namespace Engine {

	class Mouse {
	public:
		/**
		* Only returns true on first frame of key being pressed
		@param int keycode: the id of the key to check
		*/
		static bool wasButtonPressed(int buttoncode);
		/**
		* Return true on every frame of key being pressed
		@param int keycode: the id of the key to check
		*/
		static bool isButtonPressed(int buttoncode);
		static void setButtonPressed(int buttoncode, bool value);
		
		static void setPosition(int x, int y);
		static void setPosition(Vector2 pos);
		static Vector2 getPosition() {	return m_Position; };

	private:
		struct MouseButton
		{
		public:
			int m_TimesPressed = 0;
			bool m_IsPressed = false;
		};

		static Vector2 m_Position;

		inline static std::unordered_map<int, MouseButton> m_MouseButtons;
	};
}