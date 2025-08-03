#include "Button.h"

#include <glm/gtc/matrix_transform.hpp>

Button::Button(Engine::OrthographicCamera& camera, Engine::Ref<Engine::Texture2D>& texture, glm::vec3 pos, glm::vec2 scale)
	: m_Camera(camera), m_Texture(texture), m_Position(pos), m_Scale(scale)
{
}

void Button::Render()
{
	Engine::Renderer2D::DrawQuad(m_Position, m_Scale, m_Texture);
}
void Button::Render(glm::vec4 tint)
{
	Engine::Renderer2D::DrawQuad(m_Position, m_Scale, m_Texture, tint);
}

bool Button::IsHovering()
{
	if (!m_ButtonEnabled) return false;
	glm::vec2 mousePos = GetMouseGamePosition();
	return mousePos.x > m_Position.x - (m_Scale.x / 2) && mousePos.x < m_Position.x + (m_Scale.x / 2)
		&& mousePos.y > m_Position.y - (m_Scale.y / 2) && mousePos.y < m_Position.y + (m_Scale.y / 2);
}

bool Button::IsPressed(int buttoncode)
{
	if (IsHovering() && Engine::Mouse::isButtonDown(buttoncode))
		m_TimesPressed[buttoncode]++;
	else
		m_TimesPressed[buttoncode] = 0;

	return m_TimesPressed[buttoncode] != 0;
}

bool Button::WasPressed(int buttoncode)
{
	if (IsHovering() && Engine::Mouse::isButtonDown(buttoncode))
		m_TimesPressed[buttoncode]++;
	else
		m_TimesPressed[buttoncode] = 0;

	bool wasPressed = m_TimesPressed[buttoncode] == 1;
	if (wasPressed) {
		Engine::Application::getApplication()->getAudioPlayer()->PlaySound("assets/audio/activate.wav", false, 0.5);
	}
	return wasPressed;
}

glm::vec2 Button::GetMouseGamePosition() {
	glm::vec2 posVec = glm::unProject(
		glm::vec3(Engine::Mouse::getPosition().x, float(Engine::Application::getApplication()->getWindow()->GetHeight()) - Engine::Mouse::getPosition().y, 1.0f),
		glm::mat4(1.0f),
		m_Camera.GetViewProjectionMatrix(),
		glm::vec4(0.0f, 0.0f, float(Engine::Application::getApplication()->getWindow()->GetWidth()), float(Engine::Application::getApplication()->getWindow()->GetHeight()))
	);

	return posVec;
}