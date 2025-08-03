#pragma once

#include <engine.h>
#include "gameplay/Grid.h"
#include "UI/Button.h"
#include "UI/TextRendering.h"
#include "UI/InterpolationHelper.h"

using namespace game1;

class GameLayer : public Engine::Layer
{
	enum Scene {
		Menu,
		Game,
		EndRound
	};

public:
	GameLayer();
	virtual ~GameLayer() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;
	void OnUpdate(Engine::Timestep ts) override;
	void OnRender() override;
	virtual void OnImGuiRender() override;

	glm::vec2 GetMouseGamePosition();
private:
	Scene m_CurrentScene;

	Engine::OrthographicCameraController m_CameraController;
	float m_CameraZoom = 1.f;
	float m_CameraPos[2] = { 0.f, 360.f };

	Engine::Ref<Engine::Texture2D> m_GameLogo, m_InfoText, m_TargetText;

	Button *m_StartButton, *m_QuitButton, *m_RetryButton, *m_NextButton;
	Button* m_GridStartButton;

	bool m_validCircuit;

	std::unique_ptr<Grid> m_grid;
	InterpolationHelper m_CameraYAnimation, m_CameraXAnimation;
};

