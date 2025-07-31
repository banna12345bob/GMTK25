#pragma once

#include "engine/core/Application.h"

namespace Engine {

	class Renderer2D
	{
	public:
		static void Init();

		static void StartFrame();
		static void EndFrame();

		static void DrawQuad(glm::vec3 position, glm::vec3 scale, glm::vec3 colour);
		static void DrawQuad(glm::vec3 position, glm::vec3 scale, std::string filepath, glm::vec3 tintColour);
	};
}

