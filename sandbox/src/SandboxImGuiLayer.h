#pragma once
#include <engine.h>

#include <imgui/imgui.h>

class SandboxImGuiLayer : public Engine::ImGuiLayer
{
public:
	SandboxImGuiLayer()
	{
		m_ShowWindow = true;
	}

	virtual void renderImGUILayer() override
	{
		
	}
private:
	bool m_ShowImGuiDemoWindow = false;
};