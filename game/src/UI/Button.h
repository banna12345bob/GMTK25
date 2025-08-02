#pragma once

#include <engine.h>

class Button
{
public:
	Button(Engine::OrthographicCamera& camera, Engine::Ref<Engine::Texture2D>& texture, glm::vec3 pos, glm::vec2 scale);

	void Render();

	glm::vec3 GetPos() { return m_Position; }
	glm::vec2 GetScale() { return m_Scale; }

	void SetPos(glm::vec3 pos) { m_Position = pos; }
	void SetScale(glm::vec2 scale) { m_Scale = scale; }
	void SetButtonEnabled(bool enabled) { m_ButtonEnabled = enabled; }

	bool IsHovering();
	bool IsPressed(int buttoncode);
	bool WasPressed(int buttoncode);
private:
	glm::vec2 GetMouseGamePosition();
private:
	std::unordered_map<int, int> m_TimesPressed;
	Engine::Ref<Engine::Texture2D> m_Texture;
	glm::vec3 m_Position;
	glm::vec2 m_Scale;

	bool m_ButtonEnabled = true;

	Engine::OrthographicCamera& m_Camera;
};

