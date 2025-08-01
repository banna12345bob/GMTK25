#pragma once

#include <engine.h>
#include <gameplay/Grid.h>

using namespace game1;

class GameLayer : public Engine::Layer
{
public:
	GameLayer();
	virtual ~GameLayer() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;
	void OnUpdate() override;
	virtual void OnImGuiRender() override;
private:
	Engine::OrthographicCameraController m_CameraController;
	float m_CameraZoom = 1.f;
	float m_CameraPos[2] = { 0.f, 0.f };

	Grid m_grid;
};

