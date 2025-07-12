#pragma once
#include <engine.h>

#include <imgui/imgui.h>

class SandboxImGuiLayer : public Engine::ImGuiLayer
{
public:
	SandboxImGuiLayer(Engine::Application* app) 
		: m_Application(app)
	{
	}

	virtual void renderImGUILayer() override
	{
		// Begin with window. Requires window name
		ImGui::Begin("Window info");

		ImGui::Text(("FPS: " + std::to_string(m_Application->m_frameRate)).c_str());

		ImGui::SeparatorText("Window Size");
		ImGui::Text((std::string("Width: ") + std::to_string(m_Application->m_Window->GetWidth())).c_str());
		ImGui::Text((std::string("Height: ") + std::to_string(m_Application->m_Window->GetHeight())).c_str());

		ImGui::SeparatorText("Vsync");
		std::string currentVsyncMode;
		switch (m_Application->m_GraphicsAPI->GetVSync()) {
			case -1:
				currentVsyncMode = "Adaptave";
				break;
			case 0:
				currentVsyncMode = "Off";
				break;
			case 1:
				currentVsyncMode = "On";
				break;
			default:
				currentVsyncMode = "Error";
				break;
		}
		if (ImGui::Button(("Vsync Mode: " + currentVsyncMode).c_str())) {
			m_Application->m_GraphicsAPI->SetVSync((m_Application->m_GraphicsAPI->GetVSync() == 0));
		}

		// Little header/tree demo
		if (ImGui::CollapsingHeader("Demo window")) {
			if (ImGui::TreeNode("Test"))
			{
				std::string demoWindowValue = m_ShowImGuiDemoWindow ? "True" : "False";
				if (ImGui::Button(("Show demo window: " + demoWindowValue).c_str())) {
					m_ShowImGuiDemoWindow = !m_ShowImGuiDemoWindow;
				}

				ImGui::TreePop();
			}
		}

		// Remember to end the window
		ImGui::End();

		if (m_ShowImGuiDemoWindow)
			ImGui::ShowDemoWindow(&m_ShowImGuiDemoWindow);
	}
private:
	Engine::Application* m_Application;
	bool m_ShowImGuiDemoWindow = false;
};