#pragma once

#include <vector>

#include "engine/ImGui/ImGUILayer.h"

#include <SDL3/SDL.h>

namespace Engine {

	class ImGuiRenderer
	{
	public:
		ImGuiRenderer();
		~ImGuiRenderer();

		void Render();

		void registerImGuiLayer(ImGuiLayer* ImGuiLayer) { m_ImGuiLayers.push_back(ImGuiLayer); }

		void handleImGUIEvents(const SDL_Event* event);
	private:
		ImGuiLayer* getImGuiLayer(int index) { return m_ImGuiLayers[index]; }
		int getImGuiLayersSize() { return (int)m_ImGuiLayers.size(); }

		std::vector<ImGuiLayer*> m_ImGuiLayers;
	};
}

