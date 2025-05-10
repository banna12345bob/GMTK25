#include "Key.h"

namespace Engine {

	void Key::setKeyPressed(int keycode, bool value)
	{
		m_Keycodes[keycode].m_IsPressed = value;

		if (m_Keycodes[keycode].m_IsPressed)
			m_Keycodes[keycode].m_TimesPressed++;
		else
			m_Keycodes[keycode].m_TimesPressed = 0;
	}

	bool Key::isKeyPressed(int keycode)
	{
		return m_Keycodes[keycode].m_IsPressed;
	}

	bool Key::wasKeyPressed(int keycode)
	{
		if (m_Keycodes[keycode].m_TimesPressed == 0 || m_Keycodes[keycode].m_TimesPressed > 1)
			return false;
		return true;
	}
}
