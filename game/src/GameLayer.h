#pragma once

#include <engine.h>
#include "gameplay/Grid.h"
#include "UI/Button.h"
#include <chrono>
#include "UI/TextRendering.h"

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

	std::chrono::high_resolution_clock::time_point m_oldTime;

	Engine::Ref<Engine::Texture2D> m_RegularFont;

	TextRendering* m_TextRenderer;

	Button* m_TestButton;

	std::unique_ptr<Grid> m_grid;
};

