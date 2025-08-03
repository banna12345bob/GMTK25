#include "GameLayer.h"

#include "engine/core/Log.h"
#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"
#include <imgui/imgui.h>


GameLayer::GameLayer()
	: Layer("GameLayer"), m_CameraController(Engine::Application::getApplication()->getWindow()->GetWidth() / Engine::Application::getApplication()->getWindow()->GetHeight()),
	m_CurrentScene(Scene::Menu)
{
	m_CameraYAnimation.StartInterpolation(m_CameraPos[1], m_CameraPos[1], 0.f);
	m_CameraXAnimation.StartInterpolation(m_CameraPos[0], m_CameraPos[0], 0.f);
}

void GameLayer::OnAttach()
{
	Block::LoadBlockTextures();
	m_Grid = std::make_unique<Grid>(5, this);


	Engine::Application::getApplication()->getAudioPlayer()->PlaySound("assets/audio/music.wav", true, 0.45f, &m_musicSoundId);
	m_muted = false;

	Engine::Ref<Engine::Texture2D> startButton = Engine::Texture2D::Create("assets/textures/UI/start_button.png");
	m_StartButton = new Button(m_CameraController.GetCamera(), startButton, { 0, -325, 0.1 }, { 128, 128 / 2 });

	Engine::Ref<Engine::Texture2D> quitButton = Engine::Texture2D::Create("assets/textures/UI/quit_button.png");
	m_QuitButton = new Button(m_CameraController.GetCamera(), quitButton, { 0, -450, 0.1 }, { 100, 100 / 2 });

	Engine::Ref<Engine::Texture2D> retryButton = Engine::Texture2D::Create("assets/textures/UI/retry_button.png");
	m_RetryButton = new Button(m_CameraController.GetCamera(), retryButton, { 365.f, -50.f, 0.1 }, { 38 * 2, 18 * 2 });

	Engine::Ref<Engine::Texture2D> nextButton = Engine::Texture2D::Create("assets/textures/UI/next_button.png");
	m_NextButton = new Button(m_CameraController.GetCamera(), nextButton, { 365.f, -50.f, 0.1 }, { 38*2, 18*2 });

	Engine::Ref<Engine::Texture2D> gridStartButton = Engine::Texture2D::Create("assets/textures/UI/grid_start_button.png");
	m_GridStartButton = new Button(m_CameraController.GetCamera(), gridStartButton, { -128 + 34, -128 + 28, 0.3 }, { 38, 18 });

	Engine::Ref<Engine::Texture2D> mainMenuButton = Engine::Texture2D::Create("assets/textures/UI/main_menu_button.png");
	m_MainMenuButton = new Button(m_CameraController.GetCamera(), mainMenuButton, { -105, 110, 0.3 }, { 34, 14 });

	m_GameLogo = Engine::Texture2D::Create("assets/textures/UI/game_logo.png");
	m_InfoText = Engine::Texture2D::Create("assets/textures/UI/info_text.png");
	m_CurrentText = Engine::Texture2D::Create("assets/textures/UI/current_text.png");
	m_TargetText = Engine::Texture2D::Create("assets/textures/UI/target.png");
	m_SuccessText = Engine::Texture2D::Create("assets/textures/UI/success_text.png");
	m_FailText = Engine::Texture2D::Create("assets/textures/UI/fail_text.png");
	m_CreditsText = Engine::Texture2D::Create("assets/textures/UI/credits_text.png");

	m_CameraZoom = 128;
	m_CameraController.SetZoomLevel(m_CameraZoom);
	m_CameraController.setPosition({ m_CameraPos[0], m_CameraPos[1], 0.f });
}

void GameLayer::OnDetach()
{
}

void GameLayer::OnUpdate(Engine::Timestep ts)
{
	//EG_TRACE("{0}, {1}", GetMouseGamePosition().x, GetMouseGamePosition().y);

	InterpolationHelper::Update(ts);
	m_CameraPos[0] = m_CameraXAnimation.CublicEaseIn();
	m_CameraPos[1] = m_CameraYAnimation.CublicEaseIn();

	if (Engine::Key::wasKeyPressed(EG_SCANCODE_M)) {
		if (m_muted) {
			Engine::Application::getApplication()->getAudioPlayer()->SetVolume(m_musicSoundId, 0.45f);
			m_muted = false;
		}
		else {
			Engine::Application::getApplication()->getAudioPlayer()->SetVolume(m_musicSoundId, 0.0f);
			m_muted = true;
		}
	}

	m_CameraController.SetZoomLevel(m_CameraZoom);
	m_CameraController.setPosition({ m_CameraPos[0], m_CameraPos[1], 0.f });

	if (m_StartButton->IsHovering() && m_CameraPos[1] == 360.f)
		m_StartButton->SetScale(glm::vec2({ 128, 128 / 2 }) * 1.1f);
	else
		m_StartButton->SetScale({ 128, 128 / 2 });

	if (m_QuitButton->IsHovering() && (m_CurrentScene == Scene::Menu || m_CurrentScene == Scene::Win))
		m_QuitButton->SetScale(glm::vec2({ 100, 100 / 2 }) * 1.1f);
	else
		m_QuitButton->SetScale({ 100, 100 / 2 });

	if (m_GridStartButton->IsHovering() && m_CameraPos[1] == 0.f && m_ValidCircuit && !m_Grid->m_executing)
		m_GridStartButton->SetScale(glm::vec2({ 38, 19 }) * 1.1f);
	else {
		m_GridStartButton->SetScale(glm::vec2({ 38, 19 }));
	}

	if (m_MainMenuButton->IsHovering() && m_CameraPos[1] == 0.f && !m_Grid->m_executing)
		m_MainMenuButton->SetScale(glm::vec2({ 34, 14 }) * 1.1f);
	else {
		m_MainMenuButton->SetScale(glm::vec2({ 34, 14 }));
	}

	if (m_CurrentScene == EndRound) {

		if (m_WasRoundSuccess) {
			if (m_NextButton->IsHovering())
				m_NextButton->SetScale(glm::vec2({ 38 * 2, 18 * 2 }) * 1.1f);
			else {
				m_NextButton->SetScale(glm::vec2({ 38 * 2, 18 * 2 }));
			}

			if (m_NextButton->WasPressed(EG_MOUSECODE_LEFT))
				m_Grid->NextRound();
		}
		else {
			if (m_RetryButton->IsHovering())
				m_RetryButton->SetScale(glm::vec2({ 38 * 2, 18 * 2 }) * 1.1f);
			else {
				m_RetryButton->SetScale(glm::vec2({ 38 * 2, 18 * 2 }));
			}

			if (m_RetryButton->WasPressed(EG_MOUSECODE_LEFT))
				m_Grid->Retry();
		}
	}


	if (m_QuitButton->WasPressed(EG_MOUSECODE_LEFT) && (m_CurrentScene == Scene::Menu || m_CurrentScene == Scene::Win))
		Engine::Application::getApplication()->getWindow()->SetRunning(false);

	if (m_StartButton->WasPressed(EG_MOUSECODE_LEFT) && m_CurrentScene == Scene::Menu)
	{
		ChangeScene(Scene::Game, false);
	}

	if (m_GridStartButton->WasPressed(EG_MOUSECODE_LEFT) && m_CurrentScene == Scene::Game) {
		m_Grid->Start(nullptr);
	}

	if (m_MainMenuButton->WasPressed(EG_MOUSECODE_LEFT) && m_CurrentScene == Scene::Game && !m_Grid->m_executing) {
		ChangeScene(Scene::Menu, false);
	}

	if (Engine::Key::wasKeyPressed(EG_SCANCODE_ESCAPE) && m_CurrentScene == Scene::Game && !m_Grid->m_executing)
	{
		ChangeScene(Scene::Menu, false);
	}

	m_CameraController.OnUpdate();

	if (m_CurrentScene == Scene::Game && m_CameraPos[0] == 0.f && m_CameraPos[1] == 0.f) {
		m_ValidCircuit = m_Grid->CheckValidCircuit();
		m_Grid->Update(ts.GetMilliseconds());
	}
}

void GameLayer::ChangeScene(Scene scene, bool roundSuccess) {
	m_CurrentScene = scene;

	switch (scene) {
	case Menu:
		m_CameraXAnimation.StartInterpolation(m_CameraPos[0], 0.f, 1.f);
		m_CameraYAnimation.StartInterpolation(m_CameraPos[1], 360.f, 1.f);
		break;
	case Game:
		m_CameraXAnimation.StartInterpolation(m_CameraPos[0], 0.f, 1.f);
		m_CameraYAnimation.StartInterpolation(m_CameraPos[1], 0.f, 1.f);
		break;
	case EndRound:
		m_WasRoundSuccess = roundSuccess;
		if (m_WasRoundSuccess) {
			Engine::Application::getApplication()->getAudioPlayer()->PlaySound("assets/audio/success.wav", false, 0.5);
		}
		else {
			Engine::Application::getApplication()->getAudioPlayer()->PlaySound("assets/audio/fail.wav", false, 0.5);
		}
		m_CameraXAnimation.StartInterpolation(m_CameraPos[0], -360.f, 1.f);
		m_CameraYAnimation.StartInterpolation(m_CameraPos[1], 0.f, 1.f);
		break;
	case Win:
		m_CameraXAnimation.StartInterpolation(m_CameraPos[0], 0.f, 1.f);
		m_CameraYAnimation.StartInterpolation(m_CameraPos[1], -360.f, 1.f);
		m_QuitButton->SetPos({ 0.f, 300.f, 0.f });
		break;
	}
}

void GameLayer::OnRender()
{
	Engine::RenderCommand::SetClearColor({ 0.1137254902, 0.0666666667, 0.1137254902, 0 });
	Engine::RenderCommand::Clear();

	// Run every frame
	Engine::Renderer2D::BeginScene(&m_CameraController.GetCamera());

	// Main Menu
	if (m_CameraPos[1] > 0.f || m_CurrentScene == Scene::Menu)
	{
		m_StartButton->Render();
		m_QuitButton->Render();
		Engine::Renderer2D::DrawQuad({ 0, -260, 0.1 }, { 128, 128 / 2 }, m_GameLogo);
		Engine::Renderer2D::DrawQuad({ 0, -390, -0.1 }, { 500 / 2, 100 / 2 }, m_InfoText, { 1, 1, 1, 1 });
		Engine::Renderer2D::DrawQuad({ -90, -475, 0.1 }, { 192 / 3, 100 / 3 }, m_CreditsText);
	}

	// End Round
	if (m_CameraPos[0] != 0.f)
	{
		if (m_WasRoundSuccess) {
			Engine::Renderer2D::DrawQuad({ 360, 0, 0.1 }, { 45, 10 }, m_SuccessText, { 1, 1, 1, 1 });
			m_NextButton->Render();
		}
		else {
			Engine::Renderer2D::DrawQuad({ 360, 0, 0.1 }, { 47, 9 }, m_FailText, { 1, 1, 1, 1 });
			m_RetryButton->Render();
		}
	}

	// Game
	if ((m_CameraPos[1] < 360.f && m_CameraPos[0] > -360.f) || m_CurrentScene == Scene::Game)
	{
		Engine::Renderer2D::DrawQuad({ 75.f, -90.f, 0.9f }, { 46.f, 11.f }, m_CurrentText);
		Engine::Renderer2D::DrawQuad({ 75.f, -106.f, 0.9f }, { 46.f, 11.f }, m_TargetText);
		m_Grid->Draw();

		if (m_ValidCircuit) m_GridStartButton->Render();
		else m_GridStartButton->Render({ 0.6, 0.6, 0.6, 1 });

		if (m_Grid->m_executing) m_MainMenuButton->Render({ 0.6, 0.6, 0.6, 1 });
		else m_MainMenuButton->Render();
	}

	// Win screen
	if (m_CameraPos[1] > -360.f || m_CurrentScene == Scene::Win)
	{
		m_Grid->m_textRenderer->RenderText("You win!", 2, { -50.f, 450.f, 0.f }, glm::vec4(1.f), 10);
		Engine::Renderer2D::DrawQuad({ 0.f, 400.f, 0.1f }, { 192, 100 }, m_CreditsText);
		m_QuitButton->Render();
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