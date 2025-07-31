#pragma once

#include "engine/core/Application.h"

namespace Engine {

	class Renderer2D
	{
	public:
		static void Init();

		static void StartFrame() { Application::getApplication()->getRenderAPI()->StartFrame(); }
		static void EndFrame() { Application::getApplication()->getRenderAPI()->EndFrame(); }

		static void DrawQuad(glm::vec3 position, glm::vec3 scale, glm::vec3 colour) { Application::getApplication()->getRenderAPI()->RenderSquare(position, scale, colour); }
	};
}

