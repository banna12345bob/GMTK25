#pragma once

#include "engine/core/Application.h"

#include "platform/OpenGL/GLShader.h"

#include "engine/renderer/VertexArray.h"
#include "engine/renderer/Buffers.h"

namespace Engine {

	class Renderer2D
	{
	public:
		static void Init();

		static void StartFrame();
		static void EndFrame();

		static void DrawQuad(glm::vec3 position, glm::vec2 scale, glm::vec4 colour);
		static void DrawQuad(glm::vec3 position, glm::vec2 scale, std::string filepath, glm::vec3 tintColour);

	private:
		inline static GLShader* shader;
	};
}

