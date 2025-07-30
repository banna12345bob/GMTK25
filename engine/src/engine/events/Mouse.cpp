#include "Mouse.h"

#include "engine/debug/Instrumentor.h"

namespace Engine {

	void Mouse::setButtonPressed(int buttoncode, bool value)
	{
		EG_PROFILE_FUNCTION();
		m_MouseButtons[buttoncode].m_IsPressed = value;
	}

	bool Mouse::isButtonDown(int buttoncode)
	{
		EG_PROFILE_FUNCTION();
		return m_MouseButtons[buttoncode].m_IsPressed;
	}

	void Mouse::setPosition(int x, int y) {
		m_Position = Vector2(x, y);
	}

	void Mouse::setPosition(Vector2 pos) {
		m_Position = pos;
	}
}