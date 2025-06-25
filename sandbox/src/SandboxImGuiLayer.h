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

		ImGui::SeparatorText("FPS");
		ImGui::Text(std::to_string(m_Application->m_frameRate).c_str());

		ImGui::SeparatorText("Window Size");
		ImGui::Text((std::string("Width: ") + std::to_string(m_Application->m_Window->GetWidth())).c_str());
		ImGui::Text((std::string("Height: ") + std::to_string(m_Application->m_Window->GetHeight())).c_str());

		ImGui::SeparatorText("Vsync");
		switch (m_Application->m_GraphicsAPI->GetVSync()) {
			case -1:
				ImGui::Text("Adaptave");
				break;
			case 0:
				ImGui::Text("Off");
				break;
			case 1:
				ImGui::Text("On");
				break;
			default:
				ImGui::Text("Error");
				break;
		}

		ImGui::SeparatorText("Demo window");
		if (ImGui::Button("Show demo window?")) {
			m_ShowImGuiDemoWindow = !m_ShowImGuiDemoWindow;
		}
		ImGui::Text(std::string("Set to: " + std::to_string(m_ShowImGuiDemoWindow)).c_str());

		// Remember to end the window
		ImGui::End();

		if (m_ShowImGuiDemoWindow)
			ImGui::ShowDemoWindow(&m_ShowImGuiDemoWindow);
	}
private:
	Engine::Application* m_Application;
	bool m_ShowImGuiDemoWindow = false;
};