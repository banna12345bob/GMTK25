#include "GameLayer.h"

#include <glm/glm.hpp>

#include <imgui/imgui.h>

GameLayer::GameLayer()
	: Layer("GameLayer")
{
}

void GameLayer::OnAttach() // I am assuming this is this just whenever gameplay is started
{
	Block::LoadBlockData();
	m_grid = Grid(16);
}

void GameLayer::OnDetach()
{
}

void GameLayer::OnUpdate()
{
	// Run every frame
	Engine::Renderer2D::BeginScene();

	// GL_DEPTH_TEST is not enabled so be mindful of your drawing order
	//Engine::Renderer2D::DrawQuad({ 0, 0, 0 }, { 1, 1 }, { 1, 1, 1, 1 });
	//Engine::Renderer2D::DrawQuad({ -0.75, 0.75, 0.5 }, { 0.25, 0.25 }, { 1, 0, 1, 1 });

	m_grid.DrawTiles();
	// Little test grid
	/*for (float x = -1.0f; x < 1.0f; x += 0.1f)
	{
		for (float y = 1; y > -1.0f; y -= 0.1f)
		{
			Engine::Renderer2D::DrawQuad({ x, y, 0.5 }, { 0.05, 0.05, 1 }, { 0, 1, 1 });
		}
	}*/

	Engine::Renderer2D::EndScene();
}

void GameLayer::OnImGuiRender()
{
	// Any ImGui rendering code goes here
}
