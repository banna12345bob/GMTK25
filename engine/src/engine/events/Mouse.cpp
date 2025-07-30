#include "Mouse.h"

#include "engine/debug/Instrumentor.h"

namespace Engine {

	void Mouse::setButtonPressed(int buttoncode, bool value)
	{
		EG_PROFILE_FUNCTION();
		m_MouseButtons[buttoncode].m_IsPressed = value;

		if (m_MouseButtons[buttoncode].m_IsPressed)
			m_MouseButtons[buttoncode].m_TimesPressed++;
		else
			m_MouseButtons[buttoncode].m_TimesPressed = 0;
	}

	bool Mouse::isButtonPressed(int buttoncode)
	{
		EG_PROFILE_FUNCTION();
		return m_MouseButtons[buttoncode].m_IsPressed;
	}

	bool Mouse::wasButtonPressed(int buttoncode)
	{
		EG_PROFILE_FUNCTION();
		if (m_MouseButtons[buttoncode].m_TimesPressed == 0 || m_MouseButtons[buttoncode].m_TimesPressed > 1)
			return false;
		return true;
	}

	void Mouse::setPosition(int x, int y) {
		m_Position = Vector2(x, y);
	}
	void Mouse::setPosition(Vector2 pos) {
		m_Position = pos;
	}
}