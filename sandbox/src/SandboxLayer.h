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
	Engine::OrthographicCameraController m_CameraController;

	Engine::Ref<Engine::Texture2D> m_Tilesheet;
	Engine::Ref<Engine::Texture2D> m_sandBoxTexture;

	Engine::Ref<Engine::Texture2D> m_TreeTexture, m_MushroomsTexture;

	bool m_ShowImGuiWindow = true;
	bool m_ShowImGuiDemoWindow = false;
};

