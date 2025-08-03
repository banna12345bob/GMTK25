#include "GameLayer.h"

#include "engine/core/Log.h"
#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"
#include <imgui/imgui.h>


GameLayer::GameLayer()
	: Layer("GameLayer"), m_CameraController(Engine::Application::getApplication()->getWindow()->GetWidth() / Engine::Application::getApplication()->getWindow()->GetHeight()),
	m_CurrentScene(Scene::Menu)
{
	m_AnimationHelper.StartInterpolation(m_CameraPos[1], m_CameraPos[1], 0.f);
}

void GameLayer::OnAttach()
{
	Block::LoadBlockData();
	Block::LoadBlockTextures();
	m_grid = std::make_unique<Grid>(5, this);

	Engine::Ref<Engine::Texture2D> startButton = Engine::Texture2D::Create("assets/textures/UI/start_button.png");
	m_StartButton = new Button(m_CameraController.GetCamera(), startButton, { 0, -325, 0.1 }, { 128, 128 / 2 });

	Engine::Ref<Engine::Texture2D> quitButton = Engine::Texture2D::Create("assets/textures/UI/quit_button.png");
	m_QuitButton = new Button(m_CameraController.GetCamera(), quitButton, { 0, -450, 0.1 }, { 100, 100 / 2 });

	m_GameLogo = Engine::Texture2D::Create("assets/textures/UI/game_logo.png");
	m_InfoText = Engine::Texture2D::Create("assets/textures/UI/info_text.png");
	m_TargetText = Engine::Texture2D::Create("assets/textures/UI/target.png");

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

	if (m_StartButton->IsHovering() && m_CameraPos[1] == 360.f)
		m_StartButton->SetScale(glm::vec2({ 128, 128 / 2 }) * 1.1f);
	else
		m_StartButton->SetScale({ 128, 128 / 2 });

	if (m_QuitButton->IsHovering() && m_CameraPos[1] == 360.f)
		m_QuitButton->SetScale(glm::vec2({ 100, 100 / 2 }) * 1.1f);
	else
		m_QuitButton->SetScale({ 100, 100 / 2 });

	if (m_QuitButton->WasPressed(EG_MOUSECODE_LEFT) && m_CurrentScene == Scene::Menu)
		Engine::Application::getApplication()->getWindow()->SetRunning(false);

	if (m_StartButton->WasPressed(EG_MOUSECODE_LEFT) && m_CurrentScene == Scene::Menu)
	{
		m_CurrentScene = Scene::Game;
		m_AnimationHelper.StartInterpolation(m_CameraPos[1], 0.f, 1.f);
	}
	if (Engine::Key::wasKeyPressed(EG_SCANCODE_ESCAPE) && m_CurrentScene == Scene::Game)
	{
		m_CurrentScene = Scene::Menu;
		m_AnimationHelper.StartInterpolation(m_CameraPos[1], 360.f, 1.f);
	}

	m_CameraController.OnUpdate();

	if (m_CurrentScene == Scene::Game)
		m_grid->Update(ts.GetMilliseconds());
}

void GameLayer::OnRender()
{
	Engine::RenderCommand::SetClearColor({ 0.1137254902, 0.0666666667, 0.1137254902, 0 });
	Engine::RenderCommand::Clear();

	// Run every frame
	Engine::Renderer2D::BeginScene(m_CameraController.GetCamera());

	if (m_CameraPos[1] != 0.f) 
	{
		m_StartButton->Render();
		m_QuitButton->Render();
		Engine::Renderer2D::DrawQuad({ 0, -260, 0.1 }, { 128, 128/2 }, m_GameLogo, { 0.8039215686, 0.7686274510, 0.2117647059, 1 });
		Engine::Renderer2D::DrawQuad({ 0, -390, -0.1 }, { 192/2, 100/2 }, m_InfoText, {1, 1, 1, 1});
	}

	if (m_CameraPos[1] != 360.f)
	{
		Engine::Renderer2D::DrawQuad({ 75.f, -100.f, 0.9f }, { 44.f, 11.f }, m_TargetText);
		m_grid->Draw();
	}

	Engine::Renderer2D::EndScene();
}

void GameLayer::OnImGuiRender()
{
	// Any ImGui rendering code goes here
}

glm::vec2 GameLayer::GetMouseGamePosition() {
	glm::vec2 posVec = glm::unProject(
		glm::vec3(Engine::Mouse::getPosition().x, float(Engine::Application::getApplication()->getWindow()->GetHeight()) - Engine::Mouse::getPosition().y, 1.0f),
		glm::mat4(1.0f),
		m_CameraController.GetCamera().GetViewProjectionMatrix(),
		glm::vec4(0.0f, 0.0f, float(Engine::Application::getApplication()->getWindow()->GetWidth()), float(Engine::Application::getApplication()->getWindow()->GetHeight()))
	);

	return posVec;
}