#include "GameLayer.h"

#include <glm/glm.hpp>

#include <imgui/imgui.h>

GameLayer::GameLayer()
	: Layer("GameLayer"), m_CameraController(Engine::Application::getApplication()->getWindow()->GetWidth() / Engine::Application::getApplication()->getWindow()->GetHeight())
{
}

void GameLayer::OnAttach() // I am assuming this is this just whenever gameplay is started
{
	Block::LoadBlockData();
	m_grid = Grid(16);

	m_CameraController.SetZoomLevel(m_CameraZoom);
	m_CameraController.setPosition({ m_CameraPos[0], m_CameraPos[1], 0.f });
}

void GameLayer::OnDetach()
{
}

void GameLayer::OnUpdate()
{
	m_CameraController.SetZoomLevel(m_CameraZoom);
	m_CameraController.setPosition({ m_CameraPos[0], m_CameraPos[1], 0.f});

	m_CameraController.OnUpdate();

	Engine::RenderCommand::SetClearColor({ 0, 0, 0, 0 });
	Engine::RenderCommand::Clear();

	// Run every frame
	Engine::Renderer2D::BeginScene(m_CameraController.GetCamera());

	Engine::Renderer2D::DrawQuad({ 0, 0, .5f }, { 0.5f, 0.5f }, { 0, 1, 1, 1 });

	m_grid.DrawTiles();

	Engine::Renderer2D::EndScene();
}

void GameLayer::OnImGuiRender()
{
	// Any ImGui rendering code goes here
	ImGui::Begin("Camera Test");
	ImGui::SliderFloat("Zoom", &m_CameraZoom, .1f, 20.f);
	ImGui::SliderFloat2("Position", m_CameraPos, -5, 5, "%.3f", 1.0f);
	ImGui::End();
}
