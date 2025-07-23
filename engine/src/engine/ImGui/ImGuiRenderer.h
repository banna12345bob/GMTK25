#pragma once

#include <vector>

#include "engine/ImGui/ImGUILayer.h"

namespace Engine {

	class ImGuiRenderer
	{
	public:
		ImGuiRenderer();
		~ImGuiRenderer();

		void Render();

		void registerImGuiLayer(ImGuiLayer* ImGuiLayer) { m_ImGuiLayers.push_back(ImGuiLayer); }
	private:
		ImGuiLayer* getImGuiLayer(int index) { return m_ImGuiLayers[index]; }
		int getImGuiLayersSize() { return (int)m_ImGuiLayers.size(); }

		std::vector<ImGuiLayer*> m_ImGuiLayers;
	};
}

