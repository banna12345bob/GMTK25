#include "GameLayer.h"

#include "engine/core/Log.h"
#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"
#include <imgui/imgui.h>


GameLayer::GameLayer()
	: Layer("GameLayer"), m_CameraController(Engine::Application::getApplication()->getWindow()->GetWidth() / Engine::Application::getApplication()->getWindow()->GetHeight()),
	m_AnimationHelper(m_CameraPos[1])
{
}

void GameLayer::OnAttach()
{
	Block::LoadBlockData();
	Block::LoadBlockTextures();
	m_grid = std::make_unique<Grid>(5);

	Engine::Ref<Engine::Texture2D> startButton = Engine::Texture2D::Create("assets/textures/start_button.png");
	m_TestButton = new Button(m_CameraController.GetCamera(), startButton, { 0, -400, 0.1 }, { 128, 128/2 });

	Engine::Ref<Engine::Texture2D> regularFont = Engine::Texture2D::Create("assets/textures/regular_font.png");
	m_TextRenderer = new TextRendering(regularFont, { 5, 7 });

	m_GameLogo = Engine::Texture2D::Create("assets/textures/game_logo.png");

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
	m_CameraPos[1] = m_AnimationHelper.CublicEaseIn();

	m_CameraController.SetZoomLevel(m_CameraZoom);
	m_CameraController.setPosition({ m_CameraPos[0], m_CameraPos[1], 0.f });

	if (m_TestButton->IsHovering())
		m_TestButton->SetScale(glm::vec2({ 128, 128 / 2 }) * 1.1f);
	else
		m_TestButton->SetScale({ 128, 128 / 2 });

	if (m_TestButton->WasPressed(EG_MOUSECODE_LEFT))
	{
		m_TestButton->SetButtonEnabled(false);
		m_AnimationHelper.StartInterpolation(m_CameraPos[1], 0.f, 1.f);
	}
	if (Engine::Key::wasKeyPressed(EG_SCANCODE_ESCAPE))
	{
		m_TestButton->SetButtonEnabled(true);
		m_AnimationHelper.StartInterpolation(m_CameraPos[1], 360.f, 1.f);
	}

	m_CameraController.OnUpdate();

	m_grid->Update(ts.GetMilliseconds());
}

void GameLayer::OnRender()
{
	Engine::RenderCommand::SetClearColor({ 0, 0, 0, 0 });
	Engine::RenderCommand::Clear();

	// Run every frame
	Engine::Renderer2D::BeginScene(m_CameraController.GetCamera());

	Engine::Renderer2D::DrawQuad({ 0.f, -360.f, -.8f }, { 360, 360 }, { 0.1137254902, 0.0666666667, 0.1137254902, 1 });

	Engine::Renderer2D::DrawQuad({ 0, -300, 0.1 }, { 128, 128/2 }, m_GameLogo, { 0.8039215686, 0.7686274510, 0.2117647059, 1 });
	m_TestButton->Render();
	//m_TextRenderer->RenderText("Main Menu", 1.f, { 0.f, -270.f, 0.5f }, {0, 0, 0, 1}, 1.f);

	glm::vec2 pos = GameLayer::GetMouseWorldPosition();
	//EG_TRACE("POS {0}, {1}", pos.x, pos.y);

	m_grid->Draw();

	Engine::Renderer2D::EndScene();
}

void GameLayer::OnImGuiRender()
{
	// Any ImGui rendering code goes here
	ImGui::Begin("Camera Test");
	ImGui::SliderFloat("EndPos", &m_end, 0.f, 360.f);

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