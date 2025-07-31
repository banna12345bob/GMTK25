#pragma once

#include <engine.h>

class SandboxLayer : public Engine::Layer
{
public:
	SandboxLayer();
	virtual ~SandboxLayer() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;
	void OnUpdate() override;
	virtual void OnImGuiRender() override;
private:
	Engine::Ref<Engine::Texture2D> m_sandBoxTexture;
	bool m_ShowImGuiWindow = true;
	bool m_ShowImGuiDemoWindow = false;
};

