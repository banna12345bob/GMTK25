#pragma once
#include "engine/graphics/GraphicsAPI.h"

#include "GLSpriteRenderer.h"

namespace Engine {

	class OpenGLGraphicsAPI : public GraphicsAPI
	{
	public:
		OpenGLGraphicsAPI(Scope<Window>* window);
		virtual void Render() override;

	private:
		Scope<Window>* m_Window;
		GLSpriteRenderer* m_SpriteRenderer;
	};
}

