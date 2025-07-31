#include "SandboxLayer.h"

#include <glm/glm.hpp>

#include <imgui/imgui.h>

SandboxLayer::SandboxLayer()
	: Layer("SandboxLayer")
{
}

void SandboxLayer::OnAttach()
{
	m_sandBoxTexture = Engine::Texture2D::Create("assets/textures/Oak_Log.png");
}

void SandboxLayer::OnDetach()
{
}

void SandboxLayer::OnUpdate()
{
	Engine::Renderer2D::BeginScene();

	// GL_DEPTH_TEST is not enabled so be mindful of your drawing order
	Engine::Renderer2D::DrawQuad({ -0.75, 0.75, 0 }, { 0.25, 0.25 }, { 1, 0, 1, 1 });
	Engine::Renderer2D::DrawQuad({ 0, 0, -0.1 }, { 1, 1 }, m_sandBoxTexture, { 1, 0, 0, 0.75 });

	// Little test grid
	for (float x = -1.0f; x < 1.1f; x += 0.1f)
	{
		for (float y = 1; y > -1.1f; y -= 0.1f)
		{
			Engine::Renderer2D::DrawQuad({ x, y, 0 }, { 0.05, 0.05 }, { 0, 1, 1, 1 });
		}
	}

	Engine::Renderer2D::EndScene();
}

void SandboxLayer::OnImGuiRender()
{
	EG_PROFILE_FUNCTION();
	if (!m_ShowImGuiWindow)
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
