#pragma once

#include <engine.h>

class Button
{
public:
	Button(Engine::Ref<Engine::Texture2D>& texture, glm::vec3 pos, glm::vec2 scale);

	void Render(Engine::OrthographicCamera Camera);

	bool IsHovering(Engine::OrthographicCamera Camera);
	bool IsPressed(Engine::OrthographicCamera Camera);
private:
	glm::vec2 GetMouseWorldPosition(Engine::OrthographicCamera Camera);
private:
	Engine::Ref<Engine::Texture2D> m_Texture;
	glm::vec3 m_Position;
	glm::vec2 m_Scale;
	glm::vec4 m_Tint;
};

