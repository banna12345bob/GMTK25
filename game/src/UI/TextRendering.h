#pragma once

#include <engine.h>

class TextRendering
{
public:
	TextRendering(Engine::Ref<Engine::Texture2D> fontTexture, glm::vec2 characterBaseSize);

	void RenderText(std::string text, float fontSize, glm::vec3 pos, float characterPadding = 0);
private:
	glm::vec2 m_CharacterBaseSize;

	std::unordered_map<char, Engine::Ref<Engine::Texture2D>> m_Characters;
};

