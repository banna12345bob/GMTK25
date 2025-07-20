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
		EG_PROFILE_FUNCTION();
		if (!m_ShowWindow)
			return;

		// Begin with window. Requires window name
		ImGui::Begin("Window info");

		ImGui::Text(("FPS: " + std::to_string(Engine::Application::getApplication()->m_frameRate)).c_str());

		ImGui::SeparatorText("Window Size");
		ImGui::Text((std::string("Width: ") + std::to_string(Engine::Application::getApplication()->getWindow()->GetWidth())).c_str());
		ImGui::Text((std::string("Height: ") + std::to_string(Engine::Application::getApplication()->getWindow()->GetHeight())).c_str());

		ImGui::SeparatorText("Vsync");
		std::string currentVsyncMode;
		switch (Engine::Application::getApplication()->getWindow()->GetVSync()) {
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
			Engine::Application::getApplication()->getWindow()->SetVSync((Engine::Application::getApplication()->getWindow()->GetVSync() == 0));
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
	bool m_ShowImGuiDemoWindow = false;
};