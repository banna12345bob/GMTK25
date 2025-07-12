#pragma once

#include <glad/glad.h>
#include <glm/vec2.hpp>
#include <glm/mat3x3.hpp>
#include <glm/glm.hpp>

#include "platform/OpenGL/GLTexture2D.h"
#include "platform/OpenGL/GLShader.h"

namespace Engine {
	class GLSpriteRenderer {
	public:
		GLSpriteRenderer();

		// Renders all sprites.
		static void DrawSprite(GLTexture2D& texture, glm::vec2 position, glm::vec2 size, float rotate, glm::vec3 color);

		static void SetVSync(bool value);
		static int GetVSync();

	private:
		inline static GLShader shader;
		inline static GLuint vao;
	};
}