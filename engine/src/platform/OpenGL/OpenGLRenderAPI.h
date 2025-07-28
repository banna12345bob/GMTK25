#pragma once

#include "engine/renderer/RenderAPI.h"
#include "GLShader.h"


namespace Engine {

	class OpenGLRenderAPI : public RenderAPI
	{
	public:
		OpenGLRenderAPI();

		virtual void Render() override;
	private:
		GLShader* shader;
		unsigned int VAO;
	};
}

