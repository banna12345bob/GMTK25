#include "GameLayer.h"

#include "engine/core/Log.h"
#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"
#include <imgui/imgui.h>

GameLayer::GameLayer()
	: Layer("GameLayer"), m_CameraController(Engine::Application::getApplication()->getWindow()->GetWidth() / Engine::Application::getApplication()->getWindow()->GetHeight())
{
	m_LettersSubtexture = std::unordered_map<char, Engine::Ref<Engine::Texture2D>>();
}

void GameLayer::OnAttach() // I am assuming this is this just whenever gameplay is started
{
	Block::LoadBlockData();
	Block::LoadBlockTextures();
	m_grid = Grid(5);

	Engine::Ref<Engine::Texture2D> oak_log = Engine::Texture2D::Create("assets/textures/Oak_Log.png");
	m_TestButton = new Button(m_CameraController.GetCamera(), oak_log, { 0, 0, 0.1 }, { 64, 64 });

	m_RegularFont = Engine::Texture2D::Create("assets/textures/regular_font.png");
	m_LettersSubtexture['!'] = Engine::SubTexture2D::CreateFromCoords(m_RegularFont, { 0, 0 }, { 5, 7 });
	m_LettersSubtexture['\''] = Engine::SubTexture2D::CreateFromCoords(m_RegularFont, { 1, 0 }, { 5, 7 });
	m_LettersSubtexture[','] = Engine::SubTexture2D::CreateFromCoords(m_RegularFont, { 2, 0 }, { 5, 7 });
	m_LettersSubtexture['.'] = Engine::SubTexture2D::CreateFromCoords(m_RegularFont, { 3, 0 }, { 5, 7 });
	m_LettersSubtexture['?'] = Engine::SubTexture2D::CreateFromCoords(m_RegularFont, { 4, 0 }, { 5, 7 });
	m_LettersSubtexture[':'] = Engine::SubTexture2D::CreateFromCoords(m_RegularFont, { 5, 0 }, { 5, 7 });
	m_LettersSubtexture['%'] = Engine::SubTexture2D::CreateFromCoords(m_RegularFont, { 6, 0 }, { 5, 7 });

	for (int i = 0; i < 9; i++)
		m_LettersSubtexture[char(48+i)] = Engine::SubTexture2D::CreateFromCoords(m_RegularFont, {i, 1}, {5, 7});

	for (int i = 0; i < 26; i++)
		m_LettersSubtexture[char(65+i)] = Engine::SubTexture2D::CreateFromCoords(m_RegularFont, { i, 2 }, { 5, 7 });

	m_CameraZoom = glm::sqrt(std::pow(Engine::Application::getApplication()->getWindow()->GetWidth(), 2) + std::pow(Engine::Application::getApplication()->getWindow()->GetWidth(), 2)) / 4;

	m_CameraController.SetZoomLevel(m_CameraZoom);
	m_CameraController.setPosition({ m_CameraPos[0], m_CameraPos[1], 0.f });
}

void GameLayer::OnDetach()
{
}

void GameLayer::OnUpdate()
{
	m_CameraController.SetZoomLevel(m_CameraZoom);
	m_CameraController.setPosition({ m_CameraPos[0], m_CameraPos[1], 0.f });

	m_CameraController.OnUpdate();

	if (m_TestButton->WasPressed(EG_MOUSECODE_LEFT))
		m_TestButton->SetScale(m_TestButton->GetScale() * 4.f);
	if (m_TestButton->WasPressed(EG_MOUSECODE_RIGHT))
		m_TestButton->SetScale(m_TestButton->GetScale() / 4.f);
}

void GameLayer::OnRender()
{
	Engine::RenderCommand::SetClearColor({ 0, 0, 0, 0 });
	Engine::RenderCommand::Clear();

	// Run every frame
	Engine::Renderer2D::BeginScene(m_CameraController.GetCamera());
	m_TestButton->Render();

	Engine::Renderer2D::DrawQuad({ -5 * 3, 0, .5 }, glm::vec2({ 5, 7 }), m_LettersSubtexture['%']);
	Engine::Renderer2D::DrawQuad({ -5 * 2, 0, .5 }, glm::vec2({ 5, 7 }), m_LettersSubtexture['\'']);
	Engine::Renderer2D::DrawQuad({ -5 * 1, 0, .5 }, glm::vec2({ 5, 7 }), m_LettersSubtexture['0']);
	Engine::Renderer2D::DrawQuad({ 5 * 0, 0, .5 }, glm::vec2({ 5, 7 }), m_LettersSubtexture['O']);
	Engine::Renderer2D::DrawQuad({ 5 * 1, 0, .5 }, glm::vec2({ 5, 7 }), m_LettersSubtexture['A']);
	Engine::Renderer2D::DrawQuad({ 5 * 2, 0, .5 }, glm::vec2({ 5, 7 }), m_LettersSubtexture['K']);
	Engine::Renderer2D::DrawQuad({ 5 * 4, 0, .5 }, glm::vec2({ 5, 7 }), m_LettersSubtexture['L']);
	Engine::Renderer2D::DrawQuad({ 5 * 5, 0, .5 }, glm::vec2({ 5, 7 }), m_LettersSubtexture['O']);
	Engine::Renderer2D::DrawQuad({ 5 * 6, 0, .5 }, glm::vec2({ 5, 7 }), m_LettersSubtexture['G']);

	m_grid.DrawTiles();

	//glm::vec2 pos = GameLayer::GetMouseWorldPosition();
	//EG_TRACE("POS {0}, {1}", pos.x, pos.y);

	m_grid.DrawTiles();

	Engine::Renderer2D::EndScene();
}

void GameLayer::OnImGuiRender()
{
	// Any ImGui rendering code goes here
	ImGui::Begin("Camera Test");
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