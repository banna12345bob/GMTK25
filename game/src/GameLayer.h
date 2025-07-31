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
	Engine::Ref<Engine::Texture2D> m_sandBoxTexture;
	bool m_ShowImGuiWindow = true;
	bool m_ShowImGuiDemoWindow = false;

	Grid m_grid;
};

