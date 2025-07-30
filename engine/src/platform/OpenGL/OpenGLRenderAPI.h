#pragma once

#include "engine/renderer/RenderAPI.h"
#include "GLShader.h"


namespace Engine {

	class OpenGLRenderAPI : public RenderAPI
	{
	public:
		OpenGLRenderAPI();

		virtual void StartFrame() override;
		virtual void EndFrame() override;

		// Very memory leaky
		virtual void RenderSquare(glm::vec3 position, glm::vec3 scale, glm::vec3 colour) override;
	private:
		GLShader* shader;

		glm::vec4 vertices[4] = {
		{ 0.5f,  0.5f, 0.0f, 1.0f },  // top right
		{ 0.5f, -0.5f, 0.0f, 1.0f },  // bottom right
		{ -0.5f, -0.5f, 0.0f, 1.0f },  // bottom left
		{ -0.5f,  0.5f, 0.0f, 1.0f }   // top left 
		};

		unsigned int indices[6] = {
			0, 1, 3,   // first triangle
			1, 2, 3    // second triangle
		};
		
	};
}

