#include "TextRendering.h"

TextRendering::TextRendering(Engine::Ref<Engine::Texture2D> fontTexture, glm::vec2 characterBaseSize)
	: m_CharacterBaseSize(characterBaseSize)
{
	m_Characters[' '] = Engine::SubTexture2D::CreateFromCoords(fontTexture, { 0, 3 }, m_CharacterBaseSize);
	m_Characters['!'] = Engine::SubTexture2D::CreateFromCoords(fontTexture, { 0, 0 }, m_CharacterBaseSize);
	m_Characters['\''] = Engine::SubTexture2D::CreateFromCoords(fontTexture, { 1, 0 }, m_CharacterBaseSize);
	m_Characters[','] = Engine::SubTexture2D::CreateFromCoords(fontTexture, { 2, 0 }, m_CharacterBaseSize);
	m_Characters['.'] = Engine::SubTexture2D::CreateFromCoords(fontTexture, { 3, 0 }, m_CharacterBaseSize);
	m_Characters['?'] = Engine::SubTexture2D::CreateFromCoords(fontTexture, { 4, 0 }, m_CharacterBaseSize);
	m_Characters[':'] = Engine::SubTexture2D::CreateFromCoords(fontTexture, { 5, 0 }, m_CharacterBaseSize);
	m_Characters['%'] = Engine::SubTexture2D::CreateFromCoords(fontTexture, { 6, 0 }, m_CharacterBaseSize);
	m_Characters['+'] = Engine::SubTexture2D::CreateFromCoords(fontTexture, { 7, 0 }, m_CharacterBaseSize);
	m_Characters['-'] = Engine::SubTexture2D::CreateFromCoords(fontTexture, { 8, 0 }, m_CharacterBaseSize);

	for (int i = 0; i < 9; i++)
		m_Characters[char(48 + i)] = Engine::SubTexture2D::CreateFromCoords(fontTexture, { i, 1 }, m_CharacterBaseSize);

	for (int i = 0; i < 26; i++)
		m_Characters[char(65 + i)] = Engine::SubTexture2D::CreateFromCoords(fontTexture, { i, 2 }, m_CharacterBaseSize);
}

void TextRendering::RenderText(std::string text, float fontSize, glm::vec3 pos, float characterPadding)
{
	int i = 0;
	for (char character : text)
	{
		character = std::toupper(character);
		if (m_Characters.find(character) != m_Characters.end())
			Engine::Renderer2D::DrawQuad({ pos.x + (m_CharacterBaseSize.x * i) + characterPadding, pos.y, pos.z }, m_CharacterBaseSize * fontSize, m_Characters[character]);
		else
			// Draw error texture if there is an unknown character
			Engine::Renderer2D::DrawQuad({ pos.x + (m_CharacterBaseSize.x * i) + characterPadding, pos.y, pos.z }, m_CharacterBaseSize * fontSize, { 1, 0, 1, 1 });
		i++;
	}
}
