#include "GameLayer.h"

#include "engine/core/Log.h"
#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"
#include <imgui/imgui.h>


GameLayer::GameLayer()
	: Layer("GameLayer"), m_CameraController(Engine::Application::getApplication()->getWindow()->GetWidth() / Engine::Application::getApplication()->getWindow()->GetHeight())
{
	m_AnimationHelper = InterpolationHelper();
}

void GameLayer::OnAttach()
{
	Block::LoadBlockData();
	Block::LoadBlockTextures();
	m_grid = std::make_unique<Grid>(5);

	Engine::Ref<Engine::Texture2D> oak_log = Engine::Texture2D::Create("assets/textures/Oak_Log.png");
	m_TestButton = new Button(m_CameraController.GetCamera(), oak_log, { 0, 0, 0.1 }, { 64, 64 });

	m_RegularFont = Engine::Texture2D::Create("assets/textures/regular_font.png");
	m_TextRenderer = new TextRendering(m_RegularFont, { 5, 7 });

	//m_CameraZoom = glm::sqrt(std::pow(Engine::Application::getApplication()->getWindow()->GetWidth(), 2) + std::pow(Engine::Application::getApplication()->getWindow()->GetWidth(), 2)) / 4;
	m_CameraZoom = 128;

	m_CameraController.SetZoomLevel(m_CameraZoom);
	m_CameraController.setPosition({ m_CameraPos[0], m_CameraPos[1], 0.f });
}

void GameLayer::OnDetach()
{
}

void GameLayer::OnUpdate(Engine::Timestep ts)
{
	InterpolationHelper::Update(ts);

	m_CameraController.SetZoomLevel(m_CameraZoom);
	m_CameraController.setPosition({ m_CameraPos[0], m_CameraPos[1], 0.f });


	m_CameraController.OnUpdate();

	m_grid->Update(ts.GetMilliseconds());

	if (m_TestButton->WasPressed(EG_MOUSECODE_LEFT))
		m_AnimationHelper.StartInterpolation(m_TestButton->GetPos().x, m_end, m_duration);

	m_TestButton->SetPos({ m_AnimationHelper.QuadraticEaseIn(), 0.f, 0.9f});
}

void GameLayer::OnRender()
{
	Engine::RenderCommand::SetClearColor({ 0, 0, 0, 0 });
	Engine::RenderCommand::Clear();

	// Run every frame
	Engine::Renderer2D::BeginScene(m_CameraController.GetCamera());
	//m_TestButton->Render();

	//m_TextRenderer->RenderText("OAK LOG!", 1.f, { 0.f, -100.f, 0.5f });

	glm::vec2 pos = GameLayer::GetMouseWorldPosition();
	EG_TRACE("POS {0}, {1}", pos.x, pos.y);

	m_grid->Draw();

	Engine::Renderer2D::EndScene();
}

void GameLayer::OnImGuiRender()
{
	// Any ImGui rendering code goes here
	ImGui::Begin("Camera Test");
	ImGui::SliderFloat("duration", &m_duration, 0.f, 10.f);
	ImGui::SliderFloat("end", &m_end, -100.f, 100.f);
	ImGui::SliderFloat("Zoom", &m_CameraZoom, .1f, 2000.f);
	ImGui::SliderFloat2("Position", m_CameraPos, -5, 5, "%.3f", 1.0f);
	ImGui::End();
}

glm::vec2 GameLayer::GetMouseWorldPosition() {
	glm::vec2 posVec = glm::unProject(
		glm::vec3(Engine::Mouse::getPosition().x, float(Engine::Application::getApplication()->getWindow()->GetHeight()) - Engine::Mouse::getPosition().y, 1.0f),
		glm::mat4(1.0f),
		m_CameraController.GetCamera().GetViewProjectionMatrix(),
		glm::vec4(0.0f, 0.0f, float(Engine::Application::getApplication()->getWindow()->GetWidth()), float(Engine::Application::getApplication()->getWindow()->GetHeight()))
	);

	return posVec;
}