#pragma once

#include <engine.h>
#include "gameplay/Grid.h"
#include "UI/Button.h"

using namespace game1;

class GameLayer : public Engine::Layer
{
public:
	GameLayer();
	virtual ~GameLayer() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;
	void OnUpdate() override;
	void OnRender() override;
	virtual void OnImGuiRender() override;

	glm::vec2 GetMouseWorldPosition();
private:
	Engine::OrthographicCameraController m_CameraController;
	float m_CameraZoom = 1.f;
	float m_CameraPos[2] = { 0.f, 0.f };

	Engine::Ref<Engine::Texture2D> m_RegularFont;

	std::unordered_map<char, Engine::Ref<Engine::Texture2D>> m_LettersSubtexture;

	Button* m_TestButton;

	Grid m_grid;
};

