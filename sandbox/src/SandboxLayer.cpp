#include "SandboxLayer.h"

#include <glm/glm.hpp>

#include <imgui/imgui.h>

SandboxLayer::SandboxLayer()
	: Layer("SandboxLayer"), m_CameraController(Engine::Application::getApplication()->getWindow()->GetWidth() / Engine::Application::getApplication()->getWindow()->GetHeight())
{
}

void SandboxLayer::OnAttach()
{
	m_sandBoxTexture = Engine::Texture2D::Create("assets/textures/Oak_Log.png");

	m_CameraController.SetZoomLevel(5.f);

	m_CameraController.setPosition({ 1, -1, 0 });
}

void SandboxLayer::OnDetach()
{
}

void SandboxLayer::OnUpdate()
{
	m_CameraController.OnUpdate();

	Engine::Renderer2D::BeginScene(m_CameraController.GetCamera());

	// Little test grid
	for (float x = -2.0f; x < 2.f; x += 0.15f)
	{
		for (float y = 2; y > -2.f; y -= 0.15f)
		{
			Engine::Renderer2D::DrawQuad({ x, y, 0 }, { 0.1, 0.1 }, { 0, 1, 1, 1 });
		}
	}

	Engine::Renderer2D::DrawQuad({ -1, 1, 1 }, { 1, 1 }, { 1, 0, 1, 1 });
	Engine::Renderer2D::DrawQuad({ 0, 0, 1 }, { 1, 1 }, m_sandBoxTexture, { 1, 0, 0, 1 });

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
