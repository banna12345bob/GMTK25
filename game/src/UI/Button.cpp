#include "Button.h"

#include <glm/gtc/matrix_transform.hpp>

Button::Button(Engine::Ref<Engine::Texture2D>& texture, glm::vec3 pos, glm::vec2 scale)
	: m_Texture(texture), m_Position(pos), m_Scale(scale)
{
}

void Button::Render(Engine::OrthographicCamera Camera)
{
	m_Tint = { 1, 1, 1, 1 };
	if (IsPressed(Camera))
		m_Tint = { 1, 0, 0, 1 };

	Engine::Renderer2D::DrawQuad(m_Position, m_Scale, m_Texture, m_Tint);
}

bool Button::IsHovering(Engine::OrthographicCamera Camera)
{
	glm::vec2 mousePos = GetMouseWorldPosition(Camera);
	return mousePos.x > m_Position.x - (m_Scale.x / 2) && mousePos.x < m_Position.x + (m_Scale.x / 2)
		&& mousePos.y > m_Position.y - (m_Scale.y / 2) && mousePos.y < m_Position.y + (m_Scale.y / 2);
}

bool Button::IsPressed(Engine::OrthographicCamera Camera)
{
	return IsHovering(Camera) && Engine::Mouse::isButtonDown(EG_MOUSECODE_LEFT);
}

glm::vec2 Button::GetMouseWorldPosition(Engine::OrthographicCamera Camera) {
	glm::vec2 posVec = glm::unProject(
		glm::vec3(Engine::Mouse::getPosition().x, float(Engine::Application::getApplication()->getWindow()->GetHeight()) - Engine::Mouse::getPosition().y, 1.0f),
		glm::mat4(1.0f),
		Camera.GetViewProjectionMatrix(),
		glm::vec4(0.0f, 0.0f, float(Engine::Application::getApplication()->getWindow()->GetWidth()), float(Engine::Application::getApplication()->getWindow()->GetHeight()))
	);

	return posVec;
}